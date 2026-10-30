// Nova Synth installer. GPL-3.0-or-later. Windows x64, no external runtime.
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>

#define PRODUCT L"Nova Synth 0.21"
#define REGKEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\NovaSynth"
static WCHAR pluginDir[MAX_PATH], appDir[MAX_PATH], binary[MAX_PATH], uninstaller[MAX_PATH];
static const WCHAR* title=L"Instalar Nova Synth";

static void error(const WCHAR* message) {
    WCHAR out[1800], detail[512];
    DWORD code=GetLastError();
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,code,0,detail,512,NULL);
    StringCchPrintfW(out,1800,L"%ls\n\nDetalle (%lu): %ls\n\nSi FL Studio esta abierto, cierralo y vuelve a intentarlo.",message,code,detail);
    MessageBoxW(NULL,out,title,MB_OK|MB_ICONERROR);
}
static BOOL paths(void) {
    WCHAR common[MAX_PATH],programs[MAX_PATH];
    if(FAILED(SHGetFolderPathW(NULL,CSIDL_PROGRAM_FILES_COMMON,NULL,SHGFP_TYPE_CURRENT,common)) ||
       FAILED(SHGetFolderPathW(NULL,CSIDL_PROGRAM_FILES,NULL,SHGFP_TYPE_CURRENT,programs))) return FALSE;
    return SUCCEEDED(StringCchPrintfW(pluginDir,MAX_PATH,L"%ls\\VST3\\Nova Synth.vst3",common)) &&
           SUCCEEDED(StringCchPrintfW(appDir,MAX_PATH,L"%ls\\Nova Synth",programs)) &&
           SUCCEEDED(StringCchPrintfW(binary,MAX_PATH,L"%ls\\Contents\\x86_64-win\\Nova Synth.vst3",pluginDir)) &&
           SUCCEEDED(StringCchPrintfW(uninstaller,MAX_PATH,L"%ls\\Desinstalar.exe",appDir));
}
static BOOL directory(const WCHAR* path) {
    int result=SHCreateDirectoryExW(NULL,path,NULL);
    if(result==ERROR_SUCCESS || result==ERROR_ALREADY_EXISTS || result==ERROR_FILE_EXISTS) return TRUE;
    SetLastError(result); return FALSE;
}
static BOOL resourceFile(int id,const WCHAR* path) {
    HRSRC r=FindResourceW(NULL,MAKEINTRESOURCEW(id),RT_RCDATA);
    if(!r) return FALSE;
    HGLOBAL h=LoadResource(NULL,r);
    const void* data=LockResource(h);
    DWORD size=SizeofResource(NULL,r),written=0;
    WCHAR temporary[MAX_PATH];
    if(!data || !size || FAILED(StringCchPrintfW(temporary,MAX_PATH,L"%ls.new",path))) return FALSE;
    HANDLE file=CreateFileW(temporary,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    if(file==INVALID_HANDLE_VALUE) return FALSE;
    BOOL ok=WriteFile(file,data,size,&written,NULL) && written==size && FlushFileBuffers(file);
    DWORD code=ok?ERROR_SUCCESS:GetLastError(); CloseHandle(file);
    if(ok) {
        ok=MoveFileExW(temporary,path,MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH);
        if(!ok) code=GetLastError();
    }
    if(!ok) { DeleteFileW(temporary); SetLastError(code); }
    return ok;
}
static BOOL registryString(HKEY key,const WCHAR* name,const WCHAR* value) {
    LONG result=RegSetValueExW(key,name,0,REG_SZ,(const BYTE*)value,(DWORD)((lstrlenW(value)+1)*sizeof(WCHAR)));
    if(result) SetLastError(result);
    return result==ERROR_SUCCESS;
}
static BOOL install(void) {
    WCHAR folder[MAX_PATH],self[MAX_PATH],path[MAX_PATH],command[MAX_PATH+32];
    StringCchPrintfW(folder,MAX_PATH,L"%ls\\Contents\\x86_64-win",pluginDir);
    if(!directory(folder) || !directory(appDir)) { error(L"No se pudo crear la carpeta del plugin."); return FALSE; }
    // Install auxiliary files before atomically replacing the plugin binary.
    if(!GetModuleFileNameW(NULL,self,MAX_PATH) || !CopyFileW(self,uninstaller,FALSE)) {
        error(L"No se pudo crear el desinstalador."); return FALSE;
    }
    StringCchPrintfW(path,MAX_PATH,L"%ls\\LEEME.txt",appDir);
    if(!resourceFile(102,path)) { error(L"No se pudo guardar la guia."); return FALSE; }
    StringCchPrintfW(path,MAX_PATH,L"%ls\\GPL-3.0.txt",appDir);
    if(!resourceFile(103,path)) { error(L"No se pudo guardar la licencia."); return FALSE; }
    StringCchPrintfW(path,MAX_PATH,L"%ls\\Fuentes.zip",appDir);
    if(!resourceFile(104,path)) { error(L"No se pudo guardar el codigo fuente."); return FALSE; }
    StringCchPrintfW(path,MAX_PATH,L"%ls\\LICENCIAS_TERCEROS.txt",appDir);
    if(!resourceFile(105,path)) { error(L"No se pudieron guardar los avisos de terceros."); return FALSE; }
    if(!resourceFile(101,binary)) { error(L"No se pudo copiar el plugin. La version anterior, si existe, se conserva."); return FALSE; }
    HKEY key;
    LONG result=RegCreateKeyExW(HKEY_LOCAL_MACHINE,REGKEY,0,NULL,0,KEY_WRITE,NULL,&key,NULL);
    if(result) { SetLastError(result); error(L"El plugin se copio pero no se pudo registrar el desinstalador."); return FALSE; }
    StringCchPrintfW(command,MAX_PATH+32,L"\"%ls\" --uninstall",uninstaller);
    BOOL ok=registryString(key,L"DisplayName",PRODUCT) && registryString(key,L"DisplayVersion",L"0.21.0") &&
            registryString(key,L"Publisher",L"Eleander Sanchez (Bodby Olwen)") && registryString(key,L"InstallLocation",appDir) &&
            registryString(key,L"UninstallString",command);
    DWORD yes=1;
    RegSetValueExW(key,L"NoModify",0,REG_DWORD,(const BYTE*)&yes,sizeof yes);
    RegSetValueExW(key,L"NoRepair",0,REG_DWORD,(const BYTE*)&yes,sizeof yes);
    RegCloseKey(key);
    if(!ok) { error(L"El plugin se copio pero el registro de desinstalacion quedo incompleto."); return FALSE; }
    return TRUE;
}
static BOOL eraseKnown(const WCHAR* file) {
    if(DeleteFileW(file)) return TRUE;
    DWORD code=GetLastError();
    return code==ERROR_FILE_NOT_FOUND || code==ERROR_PATH_NOT_FOUND;
}
static BOOL uninstall(void) {
    WCHAR path[MAX_PATH];
    if(!eraseKnown(binary)) { error(L"No se pudo retirar el plugin."); return FALSE; }
    StringCchPrintfW(path,MAX_PATH,L"%ls\\Contents\\x86_64-win",pluginDir); RemoveDirectoryW(path);
    StringCchPrintfW(path,MAX_PATH,L"%ls\\Contents",pluginDir); RemoveDirectoryW(path);
    RemoveDirectoryW(pluginDir);
    StringCchPrintfW(path,MAX_PATH,L"%ls\\LEEME.txt",appDir); eraseKnown(path);
    StringCchPrintfW(path,MAX_PATH,L"%ls\\GPL-3.0.txt",appDir); eraseKnown(path);
    StringCchPrintfW(path,MAX_PATH,L"%ls\\Fuentes.zip",appDir); eraseKnown(path);
    StringCchPrintfW(path,MAX_PATH,L"%ls\\LICENCIAS_TERCEROS.txt",appDir); eraseKnown(path);
    // Never recursively delete the user's plugin directory or unrelated files.
    RegDeleteTreeW(HKEY_LOCAL_MACHINE,REGKEY);
    if(!DeleteFileW(uninstaller)) MoveFileExW(uninstaller,NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
    if(!RemoveDirectoryW(appDir)) MoveFileExW(appDir,NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
    MessageBoxW(NULL,L"Nova Synth se ha retirado.\n\nEl archivo del desinstalador puede permanecer hasta el siguiente reinicio.",PRODUCT,MB_OK|MB_ICONINFORMATION);
    return TRUE;
}
int WINAPI wWinMain(HINSTANCE instance,HINSTANCE previous,LPWSTR command,int show) {
    (void)instance;(void)previous;(void)show;
    if(!paths()) { MessageBoxW(NULL,L"No se pudo determinar la carpeta de instalacion.",PRODUCT,MB_OK|MB_ICONERROR); return 1; }
    BOOL remove=wcsstr(command,L"--uninstall")!=NULL;
    WCHAR own[MAX_PATH];
    if(GetModuleFileNameW(NULL,own,MAX_PATH) && lstrcmpiW(own,uninstaller)==0) remove=TRUE;
    if(remove) {
        if(MessageBoxW(NULL,L"Cerrar FL Studio antes de continuar.\n\nDeseas desinstalar Nova Synth?",PRODUCT,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)!=IDYES) return 0;
        return uninstall()?0:1;
    }
    const WCHAR* welcome=L"NOVA SYNTH 0.21 - VST3 PARA WINDOWS 64 BITS\n\n"
        L"Este instalador incluye el plugin compilado. No necesitas Git, CMake, Visual Studio ni internet.\n\n"
        L"Eleander Sanchez (Bodby Olwen). Interfaz Liquid Glass con reflejos y halos neon visibles en siete colores. Incluye Tocar, Explorar y Recrear; controles completos en Editar sonido, biblioteca personal, deshacer/rehacer, variaciones y recreacion automatica entre varios motores. Favoritos directos, 12 presets recientes por proyecto, filtros visibles y avisos legibles. Conserva 812 presets. No garantiza copias identicas ni reconoce instrumentos reales con certeza.\n\n"
        L"Se instalara en la carpeta comun de plugins VST3. Cierra FL Studio antes de continuar.\n\n"
        L"Codigo abierto bajo GPL v3 o posterior. Licencia y fuentes acompanian este paquete.\n\n"
        L"Deseas instalar Nova Synth?";
    if(MessageBoxW(NULL,welcome,title,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)!=IDYES) return 0;
    if(!install()) return 1;
    WCHAR success[1300];
    StringCchPrintfW(success,1300,L"Nova Synth se ha instalado en:\n%ls\n\nAhora abre FL Studio:\n1. Options > Manage plugins\n2. Find installed plugins\n3. Carga Nova Synth como instrumento en el Channel Rack\n\nArrastra un audio a Recrear, selecciona hasta 60 segundos en modo Contexto y pulsa Crear automaticamente. Puedes revisar notas, comparar alternativas y refinar. Explorar permite preescuchar y acceder a Mis sonidos. Editar sonido conserva todos los controles. Para tocar, activa Teclado PC y haz clic en el fondo del plugin: A W S E D F T G Y H U J K. Z/X cambia octava; Esc corta el sonido. Arrastra la esquina para cambiar el tamano.\n\nPara desinstalar: Configuracion de Windows > Aplicaciones > Nova Synth.",pluginDir);
    MessageBoxW(NULL,success,PRODUCT,MB_OK|MB_ICONINFORMATION);
    return 0;
}
