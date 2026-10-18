$ErrorActionPreference = "Stop"
$projectRoot = Split-Path -Parent $PSScriptRoot
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"

Push-Location $projectRoot
try {
    $gtkCflags = & pkg-config --cflags gtk+-3.0
    if ($LASTEXITCODE -ne 0) {
        throw "Could not read GTK compiler flags from pkg-config."
    }
    $gtkLibs = & pkg-config --libs gtk+-3.0
    if ($LASTEXITCODE -ne 0) {
        throw "Could not read GTK linker flags from pkg-config."
    }

    & gcc -std=c11 -g @($gtkCflags -split '\s+') -Iinclude -o busmate.exe `
        src\main.c src\gui.c src\admin.c src\file_handler.c src\seat_allocation.c `
    @($gtkLibs -split '\s+')
    if ($LASTEXITCODE -ne 0) {
        throw "BusMate build failed."
    }

    & .\busmate.exe
    exit $LASTEXITCODE
}
finally {
    Pop-Location
}