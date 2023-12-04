# esp32-s3-lilygo-ticker-crypto
Ticker crypto sur lilygo S3 t display V1
Ceci est la V1 il y a encore bcp de chose a faire pour les version prochaine 


Installation facile 

Telecharger le code ouvrez le code sur platformio.
Pas oubliez la bibliotheque tft_espi et dans celle ci changer dans le fichier user_setup_select la version de votre appareil dans notre cas vous devez choisir:
 #include <User_Setups/Setup206_LilyGo_T_Display_S3.h>// For the LilyGo T-Display S3 based ESP32S3 with ST7789 170 x 320 TFT
 et noubliez pas de deselectionner avec //  comme ceci   //#include <User_Setup.h>  au debut du code 
Ensuite ajouter votre wifi et mot de passe et televerser sur votre s3.
Il devrait se connecter directement a votre wifi 


voici un apercu en video https://x.com/m_mikpoly/status/1720943062677954808?s=20
