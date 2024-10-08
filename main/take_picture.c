// /**
//  * This example takes a picture every 5s and print its size on serial monitor.
//  */

// // =============================== SETUP ======================================

// // 1. Board setup (Uncomment):
// // #define BOARD_WROVER_KIT
// // #define BOARD_ESP32CAM_AITHINKER

// /**
//  * 2. Kconfig setup
//  *
//  * If you have a Kconfig file, copy the content from
//  *  https://github.com/espressif/esp32-camera/blob/master/Kconfig into it.
//  * In case you haven't, copy and paste this Kconfig file inside the src directory.
//  * This Kconfig file has definitions that allows more control over the camera and
//  * how it will be initialized.
//  */

// /**
//  * 3. Enable PSRAM on sdkconfig:
//  *
//  * CONFIG_ESP32_SPIRAM_SUPPORT=y
//  *
//  * More info on
//  * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html#config-esp32-spiram-support
//  */

// // ================================ CODE ======================================

// #include <esp_log.h>
// #include <esp_system.h>
// #include <nvs_flash.h>
// #include <sys/param.h>
// #include <string.h>

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// // support IDF 5.x
// #ifndef portTICK_RATE_MS
// #define portTICK_RATE_MS portTICK_PERIOD_MS
// #endif

// #include "esp_camera.h"

// #define BOARD_WROVER_KIT 1

// // WROVER-KIT PIN Map
// #ifdef BOARD_WROVER_KIT

// #define CAM_PIN_PWDN -1  //power down is not used
// #define CAM_PIN_RESET -1 //software reset will be performed
// #define CAM_PIN_XCLK 21
// #define CAM_PIN_SIOD 26
// #define CAM_PIN_SIOC 27

// #define CAM_PIN_D7 35
// #define CAM_PIN_D6 34
// #define CAM_PIN_D5 39
// #define CAM_PIN_D4 36
// #define CAM_PIN_D3 19
// #define CAM_PIN_D2 18
// #define CAM_PIN_D1 5
// #define CAM_PIN_D0 4
// #define CAM_PIN_VSYNC 25
// #define CAM_PIN_HREF 23
// #define CAM_PIN_PCLK 22

// #endif

// // ESP32Cam (AiThinker) PIN Map
// #ifdef BOARD_ESP32CAM_AITHINKER

// #define CAM_PIN_PWDN 32
// #define CAM_PIN_RESET -1 //software reset will be performed
// #define CAM_PIN_XCLK 0
// #define CAM_PIN_SIOD 26
// #define CAM_PIN_SIOC 27

// #define CAM_PIN_D7 35
// #define CAM_PIN_D6 34
// #define CAM_PIN_D5 39
// #define CAM_PIN_D4 36
// #define CAM_PIN_D3 21
// #define CAM_PIN_D2 19
// #define CAM_PIN_D1 18
// #define CAM_PIN_D0 5
// #define CAM_PIN_VSYNC 25
// #define CAM_PIN_HREF 23
// #define CAM_PIN_PCLK 22

// #endif

// static const char *TAG = "example:take_picture";

// #if ESP_CAMERA_SUPPORTED
// static camera_config_t camera_config = {
//     .pin_pwdn = CAM_PIN_PWDN,
//     .pin_reset = CAM_PIN_RESET,
//     .pin_xclk = CAM_PIN_XCLK,
//     .pin_sccb_sda = CAM_PIN_SIOD,
//     .pin_sccb_scl = CAM_PIN_SIOC,

//     .pin_d7 = CAM_PIN_D7,
//     .pin_d6 = CAM_PIN_D6,
//     .pin_d5 = CAM_PIN_D5,
//     .pin_d4 = CAM_PIN_D4,
//     .pin_d3 = CAM_PIN_D3,
//     .pin_d2 = CAM_PIN_D2,
//     .pin_d1 = CAM_PIN_D1,
//     .pin_d0 = CAM_PIN_D0,
//     .pin_vsync = CAM_PIN_VSYNC,
//     .pin_href = CAM_PIN_HREF,
//     .pin_pclk = CAM_PIN_PCLK,

//     //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
//     .xclk_freq_hz = 20000000,
//     .ledc_timer = LEDC_TIMER_0,
//     .ledc_channel = LEDC_CHANNEL_0,

//     .pixel_format = PIXFORMAT_RGB565, //YUV422,GRAYSCALE,RGB565,JPEG
//     .frame_size = FRAMESIZE_QVGA,    //QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

//     .jpeg_quality = 12, //0-63, for OV series camera sensors, lower number means higher quality
//     .fb_count = 1,       //When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
//     .fb_location = CAMERA_FB_IN_PSRAM,
//     .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
// };

// static esp_err_t init_camera(void)
// {
//     //initialize the camera
//     esp_err_t err = esp_camera_init(&camera_config);
//     if (err != ESP_OK)
//     {
//         ESP_LOGE(TAG, "Camera Init Failed");
//         return err;
//     }

//     return ESP_OK;
// }
// #endif

// void app_main(void)
// {
// #if ESP_CAMERA_SUPPORTED
//     if(ESP_OK != init_camera()) {
//         return;
//     }

//     while (1)
//     {
//         ESP_LOGI(TAG, "Taking picture...");
//         camera_fb_t *pic = esp_camera_fb_get();

//         // use pic->buf to access the image
//         ESP_LOGI(TAG, "Picture taken! Its size was: %zu bytes", pic->len);
//         esp_camera_fb_return(pic);

//         vTaskDelay(5000 / portTICK_RATE_MS);
//     }
// #else
//     ESP_LOGE(TAG, "Camera support is not available for this chip");
//     return;
// #endif
// }

#include "esp_camera.h"
#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_netif.h"  
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_psram.h"

// 定義攝影機引腳配置
#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27
#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

// 定義Wi-Fi配置
#define WIFI_SSID "你的Wi-Fi名稱"
#define WIFI_PASS "你的Wi-Fi密碼"

static const char *TAG = "camera_httpd";

typedef struct {
    httpd_req_t *req;
    size_t len;
} jpg_chunking_t;

// JPEG影像資料傳輸
static size_t jpg_encode_stream(void *arg, size_t index, const void *data, size_t len) {
    jpg_chunking_t *j = (jpg_chunking_t *)arg;
    if (!index) {
        j->len = 0;
    }
    if (httpd_resp_send_chunk(j->req, (const char *)data, len) != ESP_OK) {
        return 0;
    }
    j->len += len;
    return len;
}

// HTTP請求處理並回傳JPEG影像
esp_err_t jpg_httpd_handler(httpd_req_t *req) {
    camera_fb_t *fb = NULL;
    esp_err_t res = ESP_OK;
    size_t fb_len = 0;
    int64_t fr_start = esp_timer_get_time();

    fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    res = httpd_resp_set_type(req, "image/jpeg");
    if (res == ESP_OK) {
        res = httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.jpg");
    }

    if (res == ESP_OK) {
        if (fb->format == PIXFORMAT_JPEG) {
            fb_len = fb->len;
            res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
        } else {
            jpg_chunking_t jchunk = {req, 0};
            res = frame2jpg_cb(fb, 80, jpg_encode_stream, &jchunk) ? ESP_OK : ESP_FAIL;
            httpd_resp_send_chunk(req, NULL, 0);
            fb_len = jchunk.len;
        }
    }
    esp_camera_fb_return(fb);
    int64_t fr_end = esp_timer_get_time();
    ESP_LOGI(TAG, "JPG: %luKB %lums", (uint32_t)(fb_len / 1024), (uint32_t)((fr_end - fr_start) / 1000));
    return res;
}

// 初始化攝影機
void init_camera() {
    camera_config_t config;
    config.pin_pwdn = CAM_PIN_PWDN;
    config.pin_reset = CAM_PIN_RESET;
    config.pin_xclk = CAM_PIN_XCLK;
    config.pin_sccb_sda = CAM_PIN_SIOD;
    config.pin_sccb_scl = CAM_PIN_SIOC;
    config.pin_d7 = CAM_PIN_D7;
    config.pin_d6 = CAM_PIN_D6;
    config.pin_d5 = CAM_PIN_D5;
    config.pin_d4 = CAM_PIN_D4;
    config.pin_d3 = CAM_PIN_D3;
    config.pin_d2 = CAM_PIN_D2;
    config.pin_d1 = CAM_PIN_D1;
    config.pin_d0 = CAM_PIN_D0;
    config.pin_vsync = CAM_PIN_VSYNC;
    config.pin_href = CAM_PIN_HREF;
    config.pin_pclk = CAM_PIN_PCLK;
    config.xclk_freq_hz = 20000000;
    config.ledc_timer = LEDC_TIMER_0;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.pixel_format = PIXFORMAT_JPEG; // 設定為JPEG格式
    config.frame_size = FRAMESIZE_QVGA;   // 設定影像尺寸
    config.jpeg_quality = 12;             // 設定影像品質
    config.fb_count = 1;

    // 初始化攝影機
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera Init Failed");
        return;
    }
}

// 初始化 HTTP 伺服器
void start_camera_server() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t uri_handler = {
            .uri = "/capture",
            .method = HTTP_GET,
            .handler = jpg_httpd_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &uri_handler);
    }
}

// 初始化 Wi-Fi 連接
void init_wifi() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    esp_netif_init();

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Connecting to WiFi...");
    ESP_ERROR_CHECK(esp_wifi_connect());
}

void app_main() {
    init_wifi();
    vTaskDelay(2000 / portTICK_PERIOD_MS); // 確保Wi-Fi連接完成
    init_camera();                         // 初始化攝影機
    start_camera_server();                 // 啟動HTTP伺服器
    // Initialize PSRAM and camera
    if (esp_psram_is_initialized()) {
        ESP_LOGI("PSRAM", "PSRAM initialized successfully");
    } else {
        ESP_LOGE("PSRAM", "PSRAM initialization failed");
        return;
    }
}