<<<<<<< HEAD
# Set your project-specific variables here:
$projectName = "qcl_characterization_manager"  # Change to your actual executable name without .exe
$releaseDir = ".\release" # Relative path to Release folder from script location
$exePath = Join-Path $releaseDir "$projectName.exe"

# Full path to windeployqt tool in your Qt install (change to your Qt version and compiler)
$windeployqtPath = "C:\Qt\6.6.3\msvc2019_64\bin\windeployqt.exe"

# Check if windeployqt exists
if (-not (Test-Path $windeployqtPath)) {
    Write-Error "windeployqt not found at $windeployqtPath. Please update the path in the script."
    exit 1
}

# Check if executable exists
if (-not (Test-Path $exePath)) {
    Write-Error "Executable not found at $exePath. Please build your project in Release mode first."
    exit 1
}

Write-Host "Running windeployqt on $exePath ..."
& $windeployqtPath $exePath

Write-Host "windeployqt completed."

# Optional: Package folder into zip archive for easy sharing
$zipName = "$projectName-deployment.zip"
if (Test-Path $zipName) {
    Remove-Item $zipName -Force
}

Write-Host "Creating zip archive $zipName ..."
Compress-Archive -Path "$releaseDir\*" -DestinationPath $zipName

Write-Host "Deployment package created: $zipName"
Write-Host "Done."
=======
# Set your project-specific variables here:
$projectName = "qcl_characterization_manager"  # Change to your actual executable name without .exe
$releaseDir = ".\release" # Relative path to Release folder from script location
$exePath = Join-Path $releaseDir "$projectName.exe"

# Full path to windeployqt tool in your Qt install (change to your Qt version and compiler)
$windeployqtPath = "C:\Qt\6.6.3\msvc2019_64\bin\windeployqt.exe"

# Check if windeployqt exists
if (-not (Test-Path $windeployqtPath)) {
    Write-Error "windeployqt not found at $windeployqtPath. Please update the path in the script."
    exit 1
}

# Check if executable exists
if (-not (Test-Path $exePath)) {
    Write-Error "Executable not found at $exePath. Please build your project in Release mode first."
    exit 1
}

Write-Host "Running windeployqt on $exePath ..."
& $windeployqtPath $exePath

Write-Host "windeployqt completed."

# Optional: Package folder into zip archive for easy sharing
$zipName = "$projectName-deployment.zip"
if (Test-Path $zipName) {
    Remove-Item $zipName -Force
}

Write-Host "Creating zip archive $zipName ..."
Compress-Archive -Path "$releaseDir\*" -DestinationPath $zipName

Write-Host "Deployment package created: $zipName"
Write-Host "Done."
>>>>>>> 6d1d5693 (Initial commit of QCL Characterization Manager)
