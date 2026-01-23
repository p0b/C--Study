# MinGW-w64 Installer Script
# Author: Claude Code
# Date: 2026-01-22
# Description: Download, extract, and configure MinGW-w64

param(
    [string]$InstallPath = "D:\mingw64",
    [string]$DownloadUrl = "https://github.com/niXman/mingw-builds-binaries/releases/download/15.2.0-rt_v13-rev0/x86_64-15.2.0-release-posix-seh-ucrt-rt_v13-rev0.7z"
)

Write-Host "======================================" -ForegroundColor Cyan
Write-Host "MinGW-w64 Auto Installer" -ForegroundColor Cyan
Write-Host "Version: 15.2.0-rt_v13-rev0" -ForegroundColor Cyan
Write-Host "======================================" -ForegroundColor Cyan
Write-Host ""

# Step 1: Create directory
Write-Host "[1/5] Creating install directory: $InstallPath" -ForegroundColor Green
if (-not (Test-Path -Path $InstallPath)) {
    New-Item -ItemType Directory -Path $InstallPath | Out-Null
    Write-Host "      Directory created" -ForegroundColor Green
} else {
    Write-Host "      Directory exists" -ForegroundColor Yellow
}

# Step 2: Download
$downloadFile = "$InstallPath\mingw64.7z"
Write-Host "[2/5] Downloading MinGW-w64..." -ForegroundColor Green
Write-Host "      URL: $DownloadUrl" -ForegroundColor Gray

if (-not (Test-Path -Path $downloadFile)) {
    try {
        $webClient = New-Object System.Net.WebClient
        $webClient.DownloadFile($DownloadUrl, $downloadFile)
        Write-Host "      Download complete (Size: $((Get-Item $downloadFile).Length / 1MB) MB)" -ForegroundColor Green
    } catch {
        Write-Host "      Download failed: $_" -ForegroundColor Red
        Write-Host "      Trying backup method..." -ForegroundColor Yellow
        try {
            Invoke-WebRequest -Uri $DownloadUrl -OutFile $downloadFile -UseBasicParsing
            Write-Host "      Download complete" -ForegroundColor Green
        } catch {
            Write-Host "      Backup download also failed. Please check network." -ForegroundColor Red
            exit 1
        }
    }
} else {
    Write-Host "      File already exists, skipping download" -ForegroundColor Yellow
}

# Step 3: Extract
Write-Host "[3/5] Extracting to: $InstallPath" -ForegroundColor Green

$has7z = Get-Command 7z -ErrorAction SilentlyContinue

if ($has7z) {
    Write-Host "      Using 7z..." -ForegroundColor Gray
    & 7z x $downloadFile "-o$InstallPath" -y | Out-Null
} else {
    Write-Host "      Using PowerShell Expand-Archive (this might fail for .7z)..." -ForegroundColor Gray
    try {
        $7zPath = "C:\Program Files\7-Zip\7z.exe"
        if (Test-Path $7zPath) {
            & $7zPath x $downloadFile "-o$InstallPath" -y | Out-Null
        } else {
            Write-Warning "7z.exe not found. PowerShell cannot natively extract .7z files."
            Write-Warning "Please manually extract $downloadFile to $InstallPath"
            Write-Warning "Then run this script again."
        }
    } catch {
        Write-Warning "Extraction failed: $_"
    }
}

Write-Host "      Extraction step completed" -ForegroundColor Green

# Step 4: Configure PATH
Write-Host "[4/5] Configuring PATH..." -ForegroundColor Green

$binPath = "$InstallPath\mingw64\bin"
$currentPath = [Environment]::GetEnvironmentVariable("Path", "User")

if ($currentPath -notlike "*$binPath*") {
    [Environment]::SetEnvironmentVariable("Path", "$currentPath;$binPath", "User")
    Write-Host "      PATH updated: $binPath" -ForegroundColor Green
    Write-Host "      NOTE: Restart VS Code to apply changes" -ForegroundColor Yellow
} else {
    Write-Host "      PATH already configured" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "======================================" -ForegroundColor Cyan
Write-Host "Installation Complete!" -ForegroundColor Green
Write-Host "======================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Next Steps:" -ForegroundColor Yellow
Write-Host "1. Check if g++.exe exists in D:\mingw64\mingw64\bin" -ForegroundColor White
Write-Host "2. If extraction failed, manually extract D:\mingw64\mingw64.7z" -ForegroundColor White
Write-Host "3. Restart terminal and run 'g++ --version'" -ForegroundColor White
Write-Host ""
