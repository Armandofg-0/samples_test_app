<#
    Flashea el XIAO nRF52840 por su bootloader UF2.

    La placa no tiene depurador SWD, asi que los runners nrfutil/jlink no
    funcionan: hay que copiar el .uf2 a la unidad USB que expone el
    bootloader. Esa unidad solo aparece tras pulsar RESET dos veces
    seguidas, y no hay forma de provocarlo por software.
#>
param(
    [string]$Uf2 = "$PSScriptRoot\..\build\samples_test_app\zephyr\zephyr.uf2",
    [int]$TimeoutSeconds = 120
)

$ErrorActionPreference = 'Stop'

if (-not (Test-Path $Uf2)) {
    Write-Host "No encuentro $Uf2 - compila primero (west build)." -ForegroundColor Red
    exit 1
}

Write-Host "Esperando el bootloader. Pulsa RESET dos veces seguidas en la placa..." -ForegroundColor Cyan

$deadline = (Get-Date).AddSeconds($TimeoutSeconds)
while ((Get-Date) -lt $deadline) {
    foreach ($drive in Get-CimInstance Win32_LogicalDisk | Where-Object { $_.DriveType -eq 2 }) {
        $info = Join-Path $drive.DeviceID 'INFO_UF2.TXT'
        if (Test-Path $info) {
            $boardId = (Get-Content $info | Select-String '^Board-ID:').ToString().Split(':')[1].Trim()
            Write-Host "Bootloader en $($drive.DeviceID) [$($drive.VolumeName)] - $boardId" -ForegroundColor Green
            Copy-Item $Uf2 -Destination "$($drive.DeviceID)\" -Force
            Write-Host "Firmware copiado. La placa se reinicia sola." -ForegroundColor Green
            Write-Host "Ojo: al reenumerar, Windows suele asignarle un puerto COM nuevo." -ForegroundColor Yellow
            exit 0
        }
    }
    Start-Sleep -Milliseconds 500
}

Write-Host "Timeout: no aparecio ninguna unidad de bootloader." -ForegroundColor Red
Write-Host "El doble-tap debe ser rapido, en el boton junto al conector USB-C." -ForegroundColor Yellow
exit 1
