$ShaderFolder = Join-Path $PSScriptRoot "Shader"

# Correct path to fxc.exe
$FXC = "C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64\fxc.exe"

Get-ChildItem -Path $ShaderFolder -Recurse -Filter *.hlsl | ForEach-Object {
    $target = $null

    if ($_.Name -like '*.vs.hlsl') { $target = 'vs_5_0' }
    if ($_.Name -like '*.ps.hlsl') { $target = 'ps_5_0' }

    if ($target) {
        # Remove ".vs" or ".ps" from the base name
        $outputName = $_.BaseName -replace '\.(vs|ps)$', ''
        $outputFile = "$outputName.cso"

        Write-Host "Compiling $($_.FullName) → $outputFile"
        & "$FXC" /T $target /E main /Fo $outputFile $_.FullName
    }
}