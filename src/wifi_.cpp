//=======================================================================
// File:                wifi.cpp
// Author:              Marcelo Castello (https://github.com/mcastellogh)
// Licence:             GPLV3+
// Version:             1.0.0
// Date:                Julio 2024
// Info:                Part of project for Electrónica II
//=======================================================================

//--Enumerations
enum FSM_LED_STATES{ 
    LED_ON = 0, 
    LED_OFF, 
    LED_STOP, 
    LED_WAIT_ON, 
    LED_WAIT_OFF
};

#include "wifi_.h"

//--Local variables
uint32_t _wifi_crono;
uint8_t _wifi_state = RECONNECT;
uint8_t _wifi_last_state = DISCONNECTED;
uint8_t _try_wifi_reconn=1;
uint8_t _led_state = LED_OFF;
bool _flag_wifi_reconnected=0;
uint32_t _ton;
uint32_t _toff;
bool _ap_state;

//--Prototypes
void led_welcome(void);
void led_blink(uint8_t ton, uint8_t toff, uint8_t led_pin);
void led_flash(uint8_t ton, uint8_t count, uint8_t led_pin);
void led_blink_FSM(uint8_t ton, uint8_t toff, uint8_t led_pin);

//--Extern variables
extern Config config;

//--Init and connect wifi network
bool wifi_init(WiFiMode_t mode){    
    WiFi.persistent(false);
    WiFi.mode(mode);
    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(false);
    WiFi.disconnect();

    //--WiFi mode
    switch (mode){
        case WIFI_AP_STA:
            _APconnect();
            _wifi_state=_connect();
            break;
        case WIFI_AP:
            _APconnect();
            break;
        case WIFI_STA:
            _wifi_state=_connect();
            break;
        default:
            break;
    }
    if (_wifi_state==CONNECTED){
        return true;
    }else{
        return false;
    }
}

//--FMS for maintain wifi connection. Return state of connection (T/F)
bool wifi_loop(void){
  switch (_wifi_state){
    case CONNECTED:
      if (_get_wifi_status()!=CONNECTED){
        _wifi_state=DISCONNECTED;
      }
      break;

    case RECONNECT:
        Serial.print(F("WiFi reconnect "));
        Serial.println(_try_wifi_reconn);
        _try_wifi_reconn++;
        WiFi.begin(config.ssid,config.ssid_pass);
        _wifi_crono = millis();
        _wifi_state=WAITFORCONNECT;
        break;

    case DISCONNECTED:
        if(_get_wifi_status()!=CONNECTED){
          WiFi.disconnect();
          _wifi_crono = millis();
          _wifi_state=RECONNECT;
          _flag_wifi_reconnected=1;
        }else{
          _wifi_state=CONNECTED;
        }
        break;

    case WAITTORECONNECT:
      if (millis() - _wifi_crono >= RECONN_TIMEOUT){
        _wifi_state=RECONNECT;
      }
      break;

    case WAITFORCONNECT:
      if(_get_wifi_status()!=CONNECTED){
        if (millis() - _wifi_crono >= WAITFORCONNECT_TIMEOUT){
          _wifi_crono = millis();
          _wifi_state=WAITTORECONNECT;
        }
      }else{
         if(_flag_wifi_reconnected){
          Serial.println(F("WiFi reconnected!!"));
          _try_wifi_reconn=0;
          _flag_wifi_reconnected=0;
        }
        _wifi_state=CONNECTED;
      }
      break;

    default:
      break;
  }
  _FSM_state_led(_wifi_state, ONBOARD_LED_PIN);
  
  if (_wifi_state==CONNECTED){
    return true;
  }else{
    return false;
  }
}




//--Connect to wifi network. Return state of connection (T/F)
bool _connect(void){
  Serial.printf("Connecting to %s with %s password", config.ssid, config.ssid_pass);
  WiFi.begin(config.ssid, config.ssid_pass);
  _wifi_crono = millis();
  while (WiFi.status()!=WL_CONNECTED){
    Serial.print(F("."));
    _FSM_state_led(WAITFORCONNECT, ONBOARD_LED_PIN );
    delay(100);
    if (millis() - _wifi_crono >= WAITFORCONNECT_TIMEOUT){
      break;
    }
  }
  Serial.println("");
  _wifi_state=_get_wifi_status();
  _wifi_crono = millis();
  _FSM_state_led(_wifi_state,ONBOARD_LED_PIN );
  if (_wifi_state==CONNECTED){
    Serial.printf("Connected to %s with IP: ", config.ssid);
    Serial.println(WiFi.localIP());
    return true;
  }else{
      Serial.println("Not connected!");
    return false;
  }
}

//--State machine for wifi led
void _FSM_state_led(uint8_t __wifi_state, uint8_t led){
  if(_wifi_last_state!=__wifi_state){
    switch (_wifi_state){
      case CONNECTED:      
        _ton=10000;
        _toff=0;
        break;
      case DISCONNECTED:
        _ton=0;
        _toff=10000;
        break;
      case WAITTORECONNECT:
        _ton=30;
        _toff=30;
        break;
      case WAITFORCONNECT:
        _ton=200;
        _toff=200;
        break;        
      default:
        break;
    }
    _wifi_last_state=__wifi_state;
  }
  led_blink_FSM(_ton, _toff, led);
}


//--Get the wifi state for state machine. Return state (see FSM_WIFI_STATES enum in wifi.h)
uint8_t _get_wifi_status(void){
  switch (WiFi.status()){
    case WL_CONNECTED:
      if(WiFi.localIP()[3]==0){ //check if have ip number (Fail terterbio01)
        return DISCONNECTED;
      }
      return CONNECTED;
      break;
    case WL_IDLE_STATUS:
      return WAITFORCONNECT;
      break;
    case WL_NO_SSID_AVAIL:
    case WL_CONNECT_FAILED:
    case WL_CONNECTION_LOST:
    case WL_DISCONNECTED: 
      return DISCONNECTED;
      break;
    default:
      return DISCONNECTED;
      break;
  }
}

