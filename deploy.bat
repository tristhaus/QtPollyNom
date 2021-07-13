set PATH=%PATH%;C:\Qt\6.0.3\msvc2019_64\bin

IF EXIST .\Deploy-Release (
    rmdir .\Deploy-Release /s /q
)
md .\Deploy-Release

windeployqt --dir .\Deploy-Release --no-translations ..\build-QtPollyNomSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtPollyNom\release\QtPollyNom.exe
copy ..\build-QtPollyNomSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtPollyNom\release\QtPollyNom.exe .\Deploy-Release\
copy .\LICENSE .\Deploy-Release\
copy .\README.md .\Deploy-Release\

IF EXIST .\Deploy-Release-PDB (
    rmdir .\Deploy-Release-PDB /s /q
)
md .\Deploy-Release-PDB

windeployqt --pdb --dir .\Deploy-Release-PDB --no-translations ..\build-QtPollyNomSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtPollyNom\release\QtPollyNom.exe
copy ..\build-QtPollyNomSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtPollyNom\release\QtPollyNom.exe .\Deploy-Release-PDB\
copy ..\build-QtPollyNomSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtPollyNom\release\QtPollyNom.map .\Deploy-Release-PDB\
copy ..\build-QtPollyNomSuper-Desktop_Qt_6_0_3_MSVC2019_64bit-Release\QtPollyNom\release\QtPollyNom.pdb .\Deploy-Release-PDB\
copy .\LICENSE .\Deploy-Release-PDB\
copy .\README.md .\Deploy-Release-PDB\
