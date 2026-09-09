/*
 * Envio BLE minimo para XIAO nRF52840.
 *
 * Anuncia como XIAO-FLOW y notifica un paquete de datos simulados por segundo.
 * Sin sensor, sin GPIO, sin almacenamiento: solo el transporte.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>

LOG_MODULE_REGISTER(flow_sim, LOG_LEVEL_INF);

#define BT_UUID_FLOW_SVC_VAL \
	BT_UUID_128_ENCODE(0xf1a70001, 0x9b4c, 0x4f3e, 0x8b6d, 0x1c2a3d4e5f60)
#define BT_UUID_FLOW_DATA_VAL \
	BT_UUID_128_ENCODE(0xf1a70002, 0x9b4c, 0x4f3e, 0x8b6d, 0x1c2a3d4e5f60)

static const struct bt_uuid_128 flow_svc_uuid  = BT_UUID_INIT_128(BT_UUID_FLOW_SVC_VAL);
static const struct bt_uuid_128 flow_data_uuid = BT_UUID_INIT_128(BT_UUID_FLOW_DATA_VAL);

/* Paquete de 10 bytes, little-endian. */
struct __packed flow_packet {
	uint16_t seq;  /* numero de secuencia: detecta notificaciones perdidas */
	uint32_t ts;   /* segundos desde el arranque                           */
	int16_t  flow; /* centesimas de ml/min (500 = 5.00 ml/min)             */
	int16_t  temp; /* centesimas de C      (2500 = 25.00 C)                */
};

static bool notify_enabled;

static void data_ccc_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);

	notify_enabled = (value == BT_GATT_CCC_NOTIFY);
	LOG_INF("Notificaciones %s", notify_enabled ? "habilitadas" : "deshabilitadas");
}

BT_GATT_SERVICE_DEFINE(flow_svc,
	BT_GATT_PRIMARY_SERVICE(&flow_svc_uuid),
	BT_GATT_CHARACTERISTIC(&flow_data_uuid.uuid,
			       BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE,
			       NULL, NULL, NULL),
	BT_GATT_CCC(data_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

static void connected(struct bt_conn *conn, uint8_t err)
{
	ARG_UNUSED(conn);

	if (err) {
		LOG_ERR("Conexion fallida (err 0x%02x)", err);
		return;
	}

	LOG_INF("Conectado");
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	ARG_UNUSED(conn);

	/* El CCC se reinicia al desconectar; el flag debe seguirlo. */
	notify_enabled = false;
	LOG_INF("Desconectado (reason 0x%02x)", reason);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME,
		sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

/* El UUID de 128 bits va en el scan response: junto al nombre no cabe en los
 * 31 bytes del paquete de advertising.
 */
static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_FLOW_SVC_VAL),
};

/* Seno de 16 puntos escalado a +/-100, para no usar aritmetica flotante. */
static const int16_t sin_lut[16] = {
	0, 38, 71, 92, 100, 92, 71, 38, 0, -38, -71, -92, -100, -92, -71, -38,
};

int main(void)
{
	uint16_t seq = 0;
	int err;

	/* Margen para que el host enumere el puerto USB CDC antes de los
	 * primeros mensajes; si no, se pierden.
	 */
	k_msleep(2000);

	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("bt_enable fallo (err %d)", err);
		return err;
	}

	/* En NCS 2.x esta opcion se llamaba BT_LE_ADV_OPT_CONNECTABLE. */
	err = bt_le_adv_start(BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONN,
					      BT_GAP_ADV_FAST_INT_MIN_2,
					      BT_GAP_ADV_FAST_INT_MAX_2, NULL),
			      ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("bt_le_adv_start fallo (err %d)", err);
		return err;
	}

	LOG_INF("Anunciando como %s", CONFIG_BT_DEVICE_NAME);

	while (1) {
		int16_t ripple = sin_lut[seq % ARRAY_SIZE(sin_lut)];
		struct flow_packet pkt = {
			.seq = seq++,
			.ts = (uint32_t)(k_uptime_get() / 1000),
			.flow = 500 + (ripple * 50) / 100,  /* 5.00 +/- 0.50 ml/min */
			.temp = 2500 + (ripple * 20) / 100, /* 25.00 +/- 0.20 C     */
		};

		if (notify_enabled) {
			err = bt_gatt_notify(NULL, &flow_svc.attrs[1], &pkt, sizeof(pkt));
			if (err) {
				LOG_WRN("bt_gatt_notify fallo (err %d)", err);
			} else {
				LOG_INF("seq=%u flujo=%d.%02d ml/min temp=%d.%02d C",
					pkt.seq, pkt.flow / 100, pkt.flow % 100,
					pkt.temp / 100, pkt.temp % 100);
			}
		}

		k_msleep(1000);
	}

	return 0;
}
