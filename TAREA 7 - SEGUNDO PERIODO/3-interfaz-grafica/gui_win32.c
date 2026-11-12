/* gui_win32.c - Interfaz grafica (API nativa de Windows, Win32) para el programa de
   gestion de costos del vehiculo. Usa el mismo nucleo (costos.c) que la version de consola.
   No necesita instalar librerias: solo un compilador de C para Windows.
   Compilar con MinGW:  ver build.bat  (gcc ... -mwindows) */
#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "costos.h"

#define ARCHIVO "vehiculo.dat"      /* mismo archivo que usa la version de consola */

enum {
    ID_MARCA = 100, ID_MODELO, ID_PLACA, ID_ANIO, ID_ODOMETRO, ID_BTN_VEHICULO,
    ID_FECHA, ID_CATEGORIA, ID_MONTO, ID_KM, ID_LITROS, ID_DESC, ID_BTN_AGREGAR,
    ID_LISTA, ID_BTN_ELIMINAR, ID_BTN_GUARDAR, ID_BTN_CARGAR, ID_RESUMEN
};

static HINSTANCE g_inst;
static HWND g_hwnd;
static HFONT g_font_ui, g_font_mono;
static Registro g_reg;

/* ---------- conversiones UTF-8 <-> UTF-16 ---------- */
static void a_utf8(const wchar_t *w, char *dest, int n)
{
    if (WideCharToMultiByte(CP_UTF8, 0, w, -1, dest, n, NULL, NULL) == 0) dest[0] = '\0';
}
static void de_utf8(const char *s, wchar_t *dest, int n)
{
    if (MultiByteToWideChar(CP_UTF8, 0, s, -1, dest, n) == 0) dest[0] = L'\0';
}
static HWND ctl(int id) { return GetDlgItem(g_hwnd, id); }
static void leer_ctl(int id, char *dest, int n)
{
    wchar_t w[512];
    GetWindowTextW(ctl(id), w, 512);
    a_utf8(w, dest, n);
}
static void poner_ctl(int id, const char *utf8)
{
    wchar_t w[1024];
    de_utf8(utf8, w, 1024);
    SetWindowTextW(ctl(id), w);
}
static double leer_num(int id)
{
    char t[64], *p;
    leer_ctl(id, t, 64);
    for (p = t; *p; p++) if (*p == ',') *p = '.';
    return atof(t);
}
static void aviso(const char *utf8, UINT icono)
{
    wchar_t w[512];
    de_utf8(utf8, w, 512);
    MessageBoxW(g_hwnd, w, L"Costos del vehículo", MB_OK | icono);
}

/* ---------- formato de texto ---------- */
static void pad_utf8(char *dest, size_t n, const char *s, int ancho)
{
    size_t i = 0; int chars = 0;
    for (; s[i] != '\0' && i < n - 1; i++) {
        dest[i] = s[i];
        if (((unsigned char)s[i] & 0xC0) != 0x80) chars++;
    }
    while (chars < ancho && i < n - 1) { dest[i++] = ' '; chars++; }
    dest[i] = '\0';
}
static void linea_gasto(char *out, size_t n, const Gasto *g)
{
    char cat[64];
    pad_utf8(cat, sizeof cat, categoria_nombre(g->categoria), 13);
    snprintf(out, n, "%-4d %-10s %s %10.2f %8ld %7.2f  %s", g->id, g->fecha, cat,
             g->monto, g->km, g->litros, g->descripcion);
}
static void texto_resumen(char *out, size_t n)
{
    size_t len = 0; int i; double x; char cat[64];
    out[0] = '\0';
    if (g_reg.tiene_vehiculo)
        len += (size_t)snprintf(out + len, n - len, "Vehículo: %s %s (%d), placa %s, odómetro inicial %ld km\r\n",
                                g_reg.vehiculo.marca, g_reg.vehiculo.modelo, g_reg.vehiculo.anio,
                                g_reg.vehiculo.placa, g_reg.vehiculo.odometro_inicial);
    else
        len += (size_t)snprintf(out + len, n - len, "Registre primero los datos del vehículo.\r\n");
    for (i = 0; i < CAT_CANTIDAD && len < n; i++) {
        pad_utf8(cat, sizeof cat, categoria_nombre((Categoria)i), 14);
        len += (size_t)snprintf(out + len, n - len, "  %s %12.2f\r\n", cat, total_categoria(&g_reg, (Categoria)i));
    }
    if (len < n) len += (size_t)snprintf(out + len, n - len, "  %-14s %12.2f\r\n", "TOTAL", total_general(&g_reg));
    if (len < n) {
        if (costo_por_km(&g_reg, &x) == COSTOS_OK) len += (size_t)snprintf(out + len, n - len, "Costo por kilómetro: %.4f\r\n", x);
        else len += (size_t)snprintf(out + len, n - len, "Costo por kilómetro: no disponible\r\n");
    }
    if (len < n) {
        if (consumo_promedio(&g_reg, &x) == COSTOS_OK) snprintf(out + len, n - len, "Consumo promedio: %.2f km/l\r\n", x);
        else snprintf(out + len, n - len, "Consumo promedio: no disponible (se necesitan 2 o más cargas de combustible)\r\n");
    }
}

/* ---------- actualizar pantalla ---------- */
static void refrescar(void)
{
    char linea[256], resumen[2048]; wchar_t w[2048]; int i, idx;
    SendMessageW(ctl(ID_LISTA), LB_RESETCONTENT, 0, 0);
    for (i = 0; i < g_reg.cantidad; i++) {
        linea_gasto(linea, sizeof linea, &g_reg.gastos[i]);
        de_utf8(linea, w, 2048);
        idx = (int)SendMessageW(ctl(ID_LISTA), LB_ADDSTRING, 0, (LPARAM)w);
        SendMessageW(ctl(ID_LISTA), LB_SETITEMDATA, (WPARAM)idx, (LPARAM)g_reg.gastos[i].id);
    }
    texto_resumen(resumen, sizeof resumen);
    de_utf8(resumen, w, 2048);
    SetWindowTextW(ctl(ID_RESUMEN), w);
}
static void poner_vehiculo(void)
{
    char t[64];
    if (!g_reg.tiene_vehiculo) return;
    poner_ctl(ID_MARCA, g_reg.vehiculo.marca);
    poner_ctl(ID_MODELO, g_reg.vehiculo.modelo);
    poner_ctl(ID_PLACA, g_reg.vehiculo.placa);
    snprintf(t, sizeof t, "%d", g_reg.vehiculo.anio);  poner_ctl(ID_ANIO, t);
    snprintf(t, sizeof t, "%ld", g_reg.vehiculo.odometro_inicial); poner_ctl(ID_ODOMETRO, t);
}

/* ---------- acciones de los botones ---------- */
static void accion_vehiculo(void)
{
    Vehiculo v; int rc;
    memset(&v, 0, sizeof v);
    leer_ctl(ID_MARCA, v.marca, COSTOS_TXT);
    leer_ctl(ID_MODELO, v.modelo, COSTOS_TXT);
    leer_ctl(ID_PLACA, v.placa, 16);
    v.anio = (int)leer_num(ID_ANIO);
    v.odometro_inicial = (long)leer_num(ID_ODOMETRO);
    rc = vehiculo_definir(&g_reg, &v);
    if (rc != COSTOS_OK) aviso(costos_mensaje(rc), MB_ICONWARNING);
    else refrescar();
}
static void accion_agregar(void)
{
    char fecha[32], desc[256]; int cat, rc;
    leer_ctl(ID_FECHA, fecha, 32);
    leer_ctl(ID_DESC, desc, 256);
    cat = (int)SendMessageW(ctl(ID_CATEGORIA), CB_GETCURSEL, 0, 0);
    rc = gasto_agregar(&g_reg, fecha, (Categoria)cat, leer_num(ID_MONTO), (long)leer_num(ID_KM),
                       leer_num(ID_LITROS), desc, NULL);
    if (rc != COSTOS_OK) { aviso(costos_mensaje(rc), MB_ICONWARNING); return; }
    SetWindowTextW(ctl(ID_MONTO), L"");
    SetWindowTextW(ctl(ID_KM), L"");
    SetWindowTextW(ctl(ID_LITROS), L"");
    SetWindowTextW(ctl(ID_DESC), L"");
    refrescar();
}
static void accion_eliminar(void)
{
    int sel = (int)SendMessageW(ctl(ID_LISTA), LB_GETCURSEL, 0, 0);
    int id, rc;
    if (sel == LB_ERR) { aviso("Seleccione un gasto de la lista.", MB_ICONINFORMATION); return; }
    if (MessageBoxW(g_hwnd, L"¿Eliminar el gasto seleccionado?", L"Costos del vehículo",
                    MB_YESNO | MB_ICONQUESTION) != IDYES) return;
    id = (int)SendMessageW(ctl(ID_LISTA), LB_GETITEMDATA, (WPARAM)sel, 0);
    rc = gasto_eliminar(&g_reg, id);
    if (rc != COSTOS_OK) aviso(costos_mensaje(rc), MB_ICONWARNING);
    refrescar();
}
static void accion_guardar(void)
{
    int rc = registro_guardar(&g_reg, ARCHIVO);
    aviso(rc == COSTOS_OK ? "Datos guardados en vehiculo.dat." : costos_mensaje(rc),
          rc == COSTOS_OK ? MB_ICONINFORMATION : MB_ICONWARNING);
}
static void accion_cargar(void)
{
    int rc = registro_cargar(&g_reg, ARCHIVO);
    if (rc != COSTOS_OK) { aviso(costos_mensaje(rc), MB_ICONWARNING); return; }
    poner_vehiculo();
    refrescar();
}

/* ---------- creacion de controles ---------- */
static HWND crear(const wchar_t *clase, const wchar_t *texto, DWORD estilo, DWORD ex,
                  int x, int y, int w, int h, int id, HFONT fuente)
{
    HWND c = CreateWindowExW(ex, clase, texto, WS_CHILD | WS_VISIBLE | estilo, x, y, w, h,
                             g_hwnd, (HMENU)(INT_PTR)id, g_inst, NULL);
    SendMessageW(c, WM_SETFONT, (WPARAM)fuente, TRUE);
    return c;
}
static void etiqueta(const wchar_t *t, int x, int y) { crear(L"STATIC", t, 0, 0, x, y + 3, 110, 18, -1, g_font_ui); }
static void campo(int id, int x, int y, int w)
{
    crear(L"EDIT", L"", ES_AUTOHSCROLL | WS_TABSTOP, WS_EX_CLIENTEDGE, x, y, w, 22, id, g_font_ui);
}
static void boton(const wchar_t *t, int id, int x, int y, int w, int h)
{
    crear(L"BUTTON", t, BS_PUSHBUTTON | WS_TABSTOP, 0, x, y, w, h, id, g_font_ui);
}

static void crear_controles(void)
{
    int i; wchar_t w[64]; char hoy[16], encabezado[256], cat[64]; time_t t = time(NULL);

    /* Vehiculo */
    crear(L"BUTTON", L" Vehículo ", BS_GROUPBOX, 0, 10, 8, 400, 168, -1, g_font_ui);
    etiqueta(L"Marca", 22, 30);        campo(ID_MARCA, 135, 30, 150);
    etiqueta(L"Modelo", 22, 56);       campo(ID_MODELO, 135, 56, 150);
    etiqueta(L"Placa", 22, 82);        campo(ID_PLACA, 135, 82, 150);
    etiqueta(L"Año", 22, 108);         campo(ID_ANIO, 135, 108, 150);
    etiqueta(L"Odómetro inicial", 22, 134); campo(ID_ODOMETRO, 135, 134, 150);
    boton(L"Guardar datos", ID_BTN_VEHICULO, 295, 30, 105, 28);

    /* Nuevo gasto */
    crear(L"BUTTON", L" Nuevo gasto ", BS_GROUPBOX, 0, 10, 184, 400, 226, -1, g_font_ui);
    etiqueta(L"Fecha (AAAA-MM-DD)", 22, 206); campo(ID_FECHA, 160, 206, 125);
    etiqueta(L"Categoría", 22, 232);
    crear(L"COMBOBOX", L"", CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, 0, 160, 232, 150, 200, ID_CATEGORIA, g_font_ui);
    etiqueta(L"Monto", 22, 258);       campo(ID_MONTO, 160, 258, 125);
    etiqueta(L"Kilometraje", 22, 284); campo(ID_KM, 160, 284, 125);
    etiqueta(L"Litros (combustible)", 22, 310); campo(ID_LITROS, 160, 310, 125);
    etiqueta(L"Descripción", 22, 336); campo(ID_DESC, 160, 336, 235);
    SendMessageW(ctl(ID_DESC), EM_LIMITTEXT, 70, 0);
    boton(L"Agregar gasto", ID_BTN_AGREGAR, 160, 370, 150, 30);

    for (i = 0; i < CAT_CANTIDAD; i++) {
        de_utf8(categoria_nombre((Categoria)i), w, 64);
        SendMessageW(ctl(ID_CATEGORIA), CB_ADDSTRING, 0, (LPARAM)w);
    }
    SendMessageW(ctl(ID_CATEGORIA), CB_SETCURSEL, 0, 0);
    strftime(hoy, sizeof hoy, "%Y-%m-%d", localtime(&t));
    poner_ctl(ID_FECHA, hoy);

    /* Lista de gastos */
    crear(L"BUTTON", L" Gastos registrados ", BS_GROUPBOX, 0, 420, 8, 450, 402, -1, g_font_ui);
    pad_utf8(cat, sizeof cat, "Categoría", 13);
    snprintf(encabezado, sizeof encabezado, "%-4s %-10s %s %10s %8s %7s  %s", "No.", "Fecha", cat, "Monto", "Km", "Litros", "Descripción");
    {   wchar_t we[256]; de_utf8(encabezado, we, 256);
        crear(L"STATIC", we, 0, 0, 430, 28, 430, 18, -1, g_font_mono); }
    crear(L"LISTBOX", L"", LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP,
          WS_EX_CLIENTEDGE, 430, 48, 430, 316, ID_LISTA, g_font_mono);
    SendMessageW(ctl(ID_LISTA), LB_SETHORIZONTALEXTENT, 720, 0);
    boton(L"Eliminar seleccionado", ID_BTN_ELIMINAR, 430, 372, 170, 28);
    boton(L"Guardar archivo", ID_BTN_GUARDAR, 612, 372, 120, 28);
    boton(L"Cargar archivo", ID_BTN_CARGAR, 740, 372, 120, 28);

    /* Resumen */
    crear(L"BUTTON", L" Resumen de costos ", BS_GROUPBOX, 0, 10, 418, 860, 180, -1, g_font_ui);
    crear(L"EDIT", L"", ES_MULTILINE | ES_READONLY | WS_VSCROLL, WS_EX_CLIENTEDGE, 20, 438, 840, 150, ID_RESUMEN, g_font_mono);
}

/* ---------- ventana principal ---------- */
static LRESULT CALLBACK ProcVentana(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_CREATE:
        g_hwnd = hwnd;
        crear_controles();
        if (registro_cargar(&g_reg, ARCHIVO) == COSTOS_OK) poner_vehiculo();
        refrescar();
        return 0;
    case WM_COMMAND:
        if (HIWORD(wp) == BN_CLICKED) {
            switch (LOWORD(wp)) {
                case ID_BTN_VEHICULO:  accion_vehiculo(); break;
                case ID_BTN_AGREGAR:   accion_agregar();  break;
                case ID_BTN_ELIMINAR:  accion_eliminar(); break;
                case ID_BTN_GUARDAR:   accion_guardar();  break;
                case ID_BTN_CARGAR:    accion_cargar();   break;
            }
        }
        return 0;
    case WM_CLOSE:
        registro_guardar(&g_reg, ARCHIVO);      /* se guarda al salir, como en la version de consola */
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmd, int show)
{
    WNDCLASSW wc; RECT rc; MSG m; HWND ventana;
    (void)hPrev; (void)cmd;
    g_inst = hInst;
    registro_iniciar(&g_reg);
    g_font_ui = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    g_font_mono = CreateFontW(-14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                              FIXED_PITCH | FF_MODERN, L"Consolas");

    memset(&wc, 0, sizeof wc);
    wc.lpfnWndProc = ProcVentana;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = L"CostosVehiculoWnd";
    if (!RegisterClassW(&wc)) return 1;

    rc.left = 0; rc.top = 0; rc.right = 880; rc.bottom = 608;          /* area de trabajo deseada */
    AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
    ventana = CreateWindowExW(0, L"CostosVehiculoWnd", L"Gestión de costos del vehículo",
                              WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                              CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
                              NULL, NULL, hInst, NULL);
    if (!ventana) return 1;
    ShowWindow(ventana, show);
    UpdateWindow(ventana);

    while (GetMessageW(&m, NULL, 0, 0) > 0) {
        if (!IsDialogMessageW(ventana, &m)) {
            TranslateMessage(&m);
            DispatchMessageW(&m);
        }
    }
    registro_liberar(&g_reg);
    return (int)m.wParam;
}
