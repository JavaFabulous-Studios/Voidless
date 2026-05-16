@echo off
echo Cleaning up old processes...
taskkill /F /IM node.exe >nul 2>&1
taskkill /F /IM Voidless.exe >nul 2>&1
echo Starting Voidless Launcher Development Server...
cd voidless-launcher
start cmd /k "npm run dev"
timeout /t 5
start http://localhost:3000
exit
