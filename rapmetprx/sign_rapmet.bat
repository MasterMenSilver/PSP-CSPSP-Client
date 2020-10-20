@echo off
fix-relocations.exe rapmet.prx
prxEncrypter.exe rapmet.prx
rm data.psp
echo "Signed! Copy it to data folder."
pause