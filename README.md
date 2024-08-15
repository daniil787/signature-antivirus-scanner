# Signature antivirus scanner
Works throuhgt command line
Keep command for scan by directories and add signature

## How work whith it
You need to open cmd and enter the command: 

1) cd "path project"\x64\debug 
   
3) AnivirusScanner.exe addrecord "path to malware(or test file) file"."name"."extention"   avbase.avb  "offset"  "byte sequence"  "name malware".
   Example: AnivirusScanner.exe addrecord "path to malware" avbase.avb  64 255 Trojan.WIN64.testFile
   
3)  AnivirusScanner.exe scan "Repository to scan"

