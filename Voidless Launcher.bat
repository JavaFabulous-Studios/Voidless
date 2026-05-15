@echo off
echo Starting Voidless Launcher Development Server...
cd voidless-launcher
start cmd /k "npm run dev"
timeout /t 5
start http://localhost:3000
exit
