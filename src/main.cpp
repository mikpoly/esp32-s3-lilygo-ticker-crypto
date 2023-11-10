#include <Button2.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
#define BUTTON_PIN 14
Button2 button = Button2(BUTTON_PIN);

bool page1Active = true;
const char* ssid = "YOURWIFINAME";
const char* password = "YOURWIFIPASSWORD";

const char* coinGeckoAPIURL = "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin,ethereum,tron,binancecoin,solana,kaspa,flux,swissborg,neurai,monero,avalanche-2&vs_currencies=usd";
const char* coinGeckoMarketURL = "https://api.coingecko.com/api/v3/coins/markets?vs_currency=usd&ids=bitcoin,ethereum,tron,binancecoin,solana,kaspa,flux,swissborg,neurai,monero,avalanche-2&order=market_cap_desc&per_page=100&page=1&sparkline=false&price_change_percentage=24h&locale=en";

float lastBTCPrice = 0.0;
float lastETHPrice = 0.0;
float lastTRXPrice = 0.0;
float lastBNBPrice = 0.0;
float lastSOLPrice = 0.0;
float lastKASPrice = 0.0;
float lastFLUXPrice = 0.0;
float lastCHSBPrice = 0.0;
float lastXNAPrice = 0.0;
float lastXMRPrice = 0.0;
float lastAVAXPrice = 0.0;

float getBitcoinPrice();
float getEthereumPrice();
float getTronPrice();
float getBinanceCoinPrice();
float getSolanaPrice();
float getKaspaPrice();
float getFluxPrice();
float getCHSBPrice();
float getXNAPrice();
float getXMRPrice();
float getAVAXPrice();

void displayCryptoPrice(const char* cryptoName, float price, int position, float& lastPrice, uint16_t textColor);

void drawArrow(int x, int y, bool isIncrease, uint16_t arrowColor);

void setup() {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Wi-Fi Connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
  
  // Ajouter l'inscription "Created by mikpoly" en bas de la première page
  if (page1Active) {
    tft.setTextSize(1);
    tft.setCursor(10, 150); // Ajustez la position verticale selon vos besoins
    tft.setTextColor(TFT_WHITE); // Couleur du texte
    tft.print("Ticker Created by mikpoly Twitter @ m_mikpoly");
  }

  button.setPressedHandler([](Button2& b) {
    page1Active = !page1Active;
    tft.fillScreen(TFT_BLACK);

    if (page1Active) {
      // Page 1: Bitcoin, Ethereum, TRON, Binance Coin, Solana
      displayCryptoPrice("BTC", getBitcoinPrice(), 0, lastBTCPrice, TFT_ORANGE);
      displayCryptoPrice("ETH", getEthereumPrice(), 1, lastETHPrice, TFT_MAGENTA);
      displayCryptoPrice("TRX", getTronPrice(), 2, lastTRXPrice, TFT_RED);
      displayCryptoPrice("BNB", getBinanceCoinPrice(), 3, lastBNBPrice, TFT_YELLOW);
      displayCryptoPrice("SOL", getSolanaPrice(), 4, lastSOLPrice, TFT_PURPLE);
    } else {
      // Page 2: Kaspa, Flux, SwissBorg, Neurai, Monero, Avalanche
      displayCryptoPrice("KAS", getKaspaPrice(), 0, lastKASPrice, TFT_CYAN);
      displayCryptoPrice("FLUX", getFluxPrice(), 1, lastFLUXPrice, TFT_SKYBLUE);
      displayCryptoPrice("CHSB", getCHSBPrice(), 2, lastCHSBPrice, TFT_GREENYELLOW);
      displayCryptoPrice("XNA", getXNAPrice(), 3, lastXNAPrice, TFT_MAGENTA);
      displayCryptoPrice("XMR", getXMRPrice(), 4, lastXMRPrice, TFT_ORANGE);
      displayCryptoPrice("AVAX", getAVAXPrice(), 5, lastAVAXPrice, TFT_MAGENTA);
    }
  });

  // Display initial page
  if (page1Active) {
    // Page 1: Bitcoin, Ethereum, TRON, Binance Coin, Solana
    displayCryptoPrice("BTC", getBitcoinPrice(), 0, lastBTCPrice, TFT_ORANGE);
    displayCryptoPrice("ETH", getEthereumPrice(), 1, lastETHPrice, TFT_RED);
    displayCryptoPrice("TRX", getTronPrice(), 2, lastTRXPrice, TFT_BLUE);
    displayCryptoPrice("BNB", getBinanceCoinPrice(), 3, lastBNBPrice, TFT_YELLOW);
    displayCryptoPrice("SOL", getSolanaPrice(), 4, lastSOLPrice, TFT_GREEN);
  } else {
    // Page 2: Kaspa, Flux, SwissBorg, Neurai, Monero, Avalanche
    displayCryptoPrice("KAS", getKaspaPrice(), 0, lastKASPrice, TFT_CYAN);
    displayCryptoPrice("FLUX", getFluxPrice(), 1, lastFLUXPrice, TFT_MAGENTA);
    displayCryptoPrice("CHSB", getCHSBPrice(), 2, lastCHSBPrice, TFT_WHITE);
    displayCryptoPrice("XNA", getXNAPrice(), 3, lastXNAPrice, TFT_YELLOW);
    displayCryptoPrice("XMR", getXMRPrice(), 4, lastXMRPrice, TFT_MAGENTA);
    displayCryptoPrice("AVAX", getAVAXPrice(), 5, lastAVAXPrice, TFT_MAGENTA);
  }
}

void loop() {
  button.loop();
}

float getBitcoinPrice() {
  HTTPClient http;
  if (http.begin(coinGeckoAPIURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float btcPrice = doc["bitcoin"]["usd"].as<float>();
        if (btcPrice != 0.0) {
          lastBTCPrice = btcPrice;
        }
        return btcPrice;
      }
    }
  }
  return lastBTCPrice;
}

// Les fonctions getEthereumPrice, getTronPrice, getBinanceCoinPrice, getSolanaPrice, getKaspaPrice, getFluxPrice, getCHSBPrice, getXNAPrice, getXMRPrice et getAVAXPrice suivent un modèle similaire à getBitcoinPrice.


float getEthereumPrice() {
  HTTPClient http;
  if (http.begin(coinGeckoAPIURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float ethPrice = doc["ethereum"]["usd"].as<float>();
        if (ethPrice != 0.0) {
          lastETHPrice = ethPrice;
        }
        return ethPrice;
      }
    }
  }
  return lastETHPrice;
}

float getTronPrice() {
  HTTPClient http;
  if (http.begin(coinGeckoAPIURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float tronPrice = doc["tron"]["usd"].as<float>();
        if (tronPrice != 0.0) {
          lastTRXPrice = tronPrice;
        }
        return tronPrice;
      }
    }
  }
  return lastTRXPrice;
}

float getBinanceCoinPrice() {
  HTTPClient http;
  if (http.begin(coinGeckoAPIURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float bnbPrice = doc["binancecoin"]["usd"].as<float>();
        if (bnbPrice != 0.0) {
          lastBNBPrice = bnbPrice;
        }
        return bnbPrice;
      }
    }
  }
  return lastBNBPrice;
}

float getSolanaPrice() {
  HTTPClient http;
  if (http.begin(coinGeckoAPIURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float solPrice = doc["solana"]["usd"].as<float>();
        if (solPrice != 0.0) {
          lastSOLPrice = solPrice;
        }
        return solPrice;
      }
    }
  }
  return lastSOLPrice;
}

float getKaspaPrice() {
  HTTPClient http;
  if (http.begin(coinGeckoAPIURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float kaspaPrice = doc["kaspa"]["usd"].as<float>();
        if (kaspaPrice != 0.0) {
          lastKASPrice = kaspaPrice;
        }
        return kaspaPrice;
      }
    }
  }
  return lastKASPrice;
}

float getFluxPrice() {
  HTTPClient http;
  if (http.begin(coinGeckoAPIURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float fluxPrice = doc["flux"]["usd"].as<float>();
        if (fluxPrice != 0.0) {
          lastFLUXPrice = fluxPrice;
        }
        return fluxPrice;
      }
    }
  }
  return lastFLUXPrice;
}

float getCHSBPrice() {
  HTTPClient http;
  if (http.begin(coinGeckoAPIURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float chsbPrice = doc["swissborg"]["usd"].as<float>();
        if (chsbPrice != 0.0) {
          lastCHSBPrice = chsbPrice;
        }
        return chsbPrice;
      }
    }
  }
  return lastCHSBPrice;
}

float getXNAPrice() {
  HTTPClient http;
  if (http.begin(coinGeckoAPIURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float xnaPrice = doc["neurai"]["usd"].as<float>();
        if (xnaPrice != 0.0) {
          lastXNAPrice = xnaPrice;
        }
        return xnaPrice;
      }
    }
  }
  return lastXNAPrice;
}

float getXMRPrice() {
  HTTPClient http;
  if (http.begin(coinGeckoAPIURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float xmrPrice = doc["monero"]["usd"].as<float>();
        if (xmrPrice != 0.0) {
          lastXMRPrice = xmrPrice;
        }
        return xmrPrice;
      }
    }
  }
  return lastXMRPrice;
}

float getAVAXPrice() {
  HTTPClient http;
  if (http.begin(coinGeckoAPIURL)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float avaxPrice = doc["avalanche"]["usd"].as<float>();
        if (avaxPrice != 0.0) {
          lastAVAXPrice = avaxPrice;
        }
        return avaxPrice;
      }
    }
  }
  return lastAVAXPrice;
}







void displayCryptoPrice(const char* cryptoName, float price, int position, float& lastPrice, uint16_t textColor) {
  tft.setTextSize(2);
  int y = 30 + (position - 1) * 30; // Position verticale en fonction de la crypto-monnaie
  tft.setCursor(10, y);
  tft.setTextColor(textColor); // Couleur du texte
  tft.print(cryptoName);

  tft.setCursor(220, y); // Position du prix
  tft.print("$");
  tft.setCursor(80, y); // Position du prix
  tft.print(price, 5); // Afficher le prix avec 5 décimales
  lastPrice = price;
}

