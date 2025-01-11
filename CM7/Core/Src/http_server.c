#include <string.h>
#include "http_server.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "cmsis_os.h"
#include "stm32h7xx.h"
#include "FreeRTOS.h"
#include "task.h"

#define WEBSERVER_THREAD_PRIO    ( tskIDLE_PRIORITY + 4 )

static const char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static const char http_404_hdr[] = "HTTP/1.1 404 ERROR\r\nContent-type: text/html\r\n\r\n";

void get_board_info(char *buffer, size_t size) {
    size_t heapSize = xPortGetFreeHeapSize();
    size_t totalHeapSize = configTOTAL_HEAP_SIZE;
    size_t totalRAM = 512 * 1024;
    size_t usedRAM = totalRAM - heapSize;

    snprintf(buffer, size,
        "<html><head><title>STM32H745ZI-Q Info</title></head>"
        "<body><h1>STM32H745ZI-Q Information</h1>"
        "<p>Microcontroller: STM32H745ZI-Q</p>"
        "<p>CPU Frequency: %u MHz</p>"
        "<p>Flash Size: %u KB</p>"
        "<p>RAM Size: %u KB</p>"
        "<p>Free Heap Size: %u bytes</p>"
        "<p>Total Heap Size: %u bytes</p>"
        "<p>Used RAM: %u bytes</p>"
        "</body></html>",
        SystemCoreClock / 1000000,
        2048UL,
        totalRAM / 1024UL,
        heapSize,
        totalHeapSize,
        usedRAM
    );
}

/** Serve one HTTP connection accepted in the http thread */
static void http_server_netconn_serve(struct netconn *conn) {
    struct netbuf *inbuf;
    char *buf;
    u16_t buflen;
    err_t err;

    /* Read the data from the port, blocking if nothing yet there. */
    err = netconn_recv(conn, &inbuf);

    if (err == ERR_OK) {
        netbuf_data(inbuf, (void**)&buf, &buflen);

        /* Is this an HTTP GET command? */
        if (strncmp((char const *)buf, "GET /index.html ", 16) == 0) {
            char board_info[1024]; // Увеличиваем размер буфера для HTML
            get_board_info(board_info, sizeof(board_info));

            /* Send the HTML header */
            netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
            /* Send our HTML page with board info */
            netconn_write(conn, board_info, strlen(board_info), NETCONN_NOCOPY);
        } else {
            netconn_write(conn, http_404_hdr, sizeof(http_404_hdr) - 1, NETCONN_NOCOPY);
            netconn_write(conn, "<html><head><title>Error</title></head><body><h1>404 Not Found</h1></body></html>", 100, NETCONN_NOCOPY);
 }
    }
    /* Close the connection */
    netconn_close(conn);
    /* Delete the buffer */
    netbuf_delete(inbuf);
}

/** The main function, never returns! */
static void http_server_netconn_thread(void *arg) {
    struct netconn *conn, *newconn;
    err_t err;
    LWIP_UNUSED_ARG(arg);

    /* Create a new TCP connection handle */
    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, IP_ADDR_ANY, 80);
    LWIP_ERROR("http_server: invalid conn", (conn != NULL), return;);

    /* Put the connection into LISTEN state */
    netconn_listen(conn);

    do {
        err = netconn_accept(conn, &newconn);
        if (err == ERR_OK) {
            http_server_netconn_serve(newconn);
            netconn_delete(newconn);
        }
    } while (err == ERR_OK);

    netconn_close(conn);
    netconn_delete(conn);
}

/** Initialize the HTTP server (start its thread) */
void http_server_netconn_init(void) {
    sys_thread_new("http_server_netconn", http_server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, WEBSERVER_THREAD_PRIO);
}
