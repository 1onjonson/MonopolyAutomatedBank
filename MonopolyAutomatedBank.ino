#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <string.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Экземпляр MFRC522

const byte ROWS  = 4;
const byte COLS = 4;

// Меню
const char* menuItems[] = {"Increase balance", "Reduce balance", "Show balance", "Add new card", "Delete card"};
int menuIndex = 0; // Индекс текущего пункта меню
const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', 'E'},
  {'*', '0', '#', 'C'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);  

struct Card {
  String id;
  int balance;
};

Card cards[10];  // Массив для хранения до 10 карт
int cardCount = 0;
String masterCard = "";  // Переменная для хранения ID мастер-карты

void setup() {
  SPI.begin();      // Инициализация SPI шины
  mfrc522.PCD_Init();   // Инициализация MFRC522
  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
  lcd.print("Engineers");
  lcd.setCursor(0,1);
  lcd.print(" of the future");
  delay(2000);
  lcd.clear();
  updateMenu(); // Обновляем отображение меню
}

void loop() {
  char customKey = customKeypad.getKey();
  
  // Добавление мастер-карты по нажатию комбинации клавиш "+" и "-"
  if(customKey == '*') {
    addMasterCard();
  }

  if(customKey == '+') {
    menuIndex = (menuIndex + 1) % menuLength; // Переход к следующему пункту меню
    updateMenu();
  }
  
  if(customKey == '-') {
    menuIndex = (menuIndex - 1 + menuLength) % menuLength; // Переход к предыдущему пункту меню
    updateMenu();
  }

  if(customKey == 'E') {
    executeMenuAction(); // Выполнение действия текущего пункта меню
  }

  // Дополнительные функции, такие как работа с картами, можно оставить как в предыдущем коде
}

void updateMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu:");
  lcd.setCursor(0, 1);
  lcd.print(menuItems[menuIndex]); // Отображение текущего пункта меню
}

void executeMenuAction() {
  // Проверка мастер-карты перед выполнением действия
  if (verifyMasterCard()) {
  switch(menuIndex) {
    case 0: // Increase money
      lcd.clear();
      lcd.print("Increase balance");
      // Добавьте код для увеличения денег
      updateBalance(true);
      break;
    case 1: // Reduce balance
      lcd.clear();
      lcd.print("Reduce balance");
      // Добавьте код для уменьшения баланса
      updateBalance(false);
      break;
    case 2: // Show balance
      lcd.clear();
      lcd.print("Show balance");
      // Добавьте код для показа баланса
      showBalance();
      break;
    case 3: // Add new card
      lcd.clear();
      lcd.print("Add new card");
      // Добавьте код для добавления новой карты
      addCard();
      break;
    case 4: // Delete card
      lcd.clear();
      lcd.print("Delete card");
      // Добавьте код для удаления карты
      deleteCard();
      break;
  }
  } else {
    lcd.clear();
    lcd.print("Access denied");
    delay(2000);
  }
  updateMenu(); // Возвращаемся в меню после выполнения действия
}

void addMasterCard() {
  lcd.clear();
  lcd.print("Set master card:");
  lcd.setCursor(0, 1);
  lcd.print("Scan now...");

  String cardId = scanCard();  // Сканируем карту

  if (cardId != "") {
    masterCard = cardId;
    lcd.clear();
    lcd.print("Master card set");
    lcd.setCursor(0, 1);
    lcd.print(masterCard);
    delay(2000);
  } else {
    lcd.clear();
    lcd.print("Operation canceled");
    delay(2000);
  }
  updateMenu();
}

bool verifyMasterCard() {
  lcd.clear();
  lcd.print("Scan master card:");

  String cardId = scanCard();  // Сканируем карту

  if (cardId == masterCard) {
    lcd.clear();
    lcd.print("Access granted");
    delay(1000);
    return true;
  } else {
    lcd.clear();
    lcd.print("Invalid");
    lcd.setCursor(0, 1);
    lcd.print("master card");
    delay(2000);
    return false;
  }
}

void addCard() {
  lcd.clear();
  lcd.print("Add new card:");
  lcd.setCursor(0, 1);
  lcd.print("Scan now...");

  String cardId = scanCard();  // Сканируем карту

  if (cardId != "") {
    lcd.clear();
    lcd.print("Card added:");
    lcd.setCursor(0, 1);
    lcd.print(cardId);
    delay(2000);

    if (cardCount < 10) {
      cards[cardCount].id = cardId;
      cards[cardCount].balance = 1000;  // Присваиваем баланс 1000 монет
      cardCount++;
    } else {
      lcd.clear();
      lcd.print("Card storage full");
      delay(2000);
    }
  } else {
    lcd.clear();
    lcd.print("Operation canceled");
    delay(2000);
  }
}

void showBalance() {
  lcd.clear();
  lcd.print("Show balance:");
  lcd.setCursor(0, 1);
  lcd.print("Scan card...");

  String cardId = scanCard();  // Сканируем карту

  if (cardId != "") {
    // Поиск карты по ID
    bool cardFound = false;
    for (int i = 0; i < cardCount; i++) {
      if (cards[i].id == cardId) {
        lcd.clear();
        lcd.print("Balance:");
        lcd.setCursor(0, 1);
        lcd.print(cards[i].balance);
        cardFound = true;
        delay(3000);
        break;
      }
    }

    if (!cardFound) {
      lcd.clear();
      lcd.print("Card not found");
      delay(2000);
    }
  } else {
    lcd.clear();
    lcd.print("Operation canceled");
    delay(2000);
  }
}

void updateBalance(bool increase) {
  lcd.clear();
  lcd.print(increase ? "Increase money:" : "Reduce balance:");
  lcd.setCursor(0, 1);
  lcd.print("Scan card...");

  String cardId = scanCard();  // Сканируем карту

  if (cardId != "") {
    // Поиск карты по ID
    bool cardFound = false;
    int cardIndex = -1;
    for (int i = 0; i < cardCount; i++) {
      if (cards[i].id == cardId) {
        cardFound = true;
        cardIndex = i;
        break;
      }
    }

    if (cardFound) {
      lcd.clear();
      lcd.print("Enter amount:");
      String amountStr = "";
      
      // Считываем сумму, пока не будет нажата кнопка 'E'
      while (true) {
        char customKey = customKeypad.getKey();
        if (customKey >= '0' && customKey <= '9') {
          amountStr += customKey;
          lcd.setCursor(0, 1);
          lcd.print(amountStr);
        } else if (customKey == 'E') {
          break;
        } else if (customKey == 'C') {
          lcd.clear();
          lcd.print("Operation canceled");
          delay(2000);
          return;
        }
      }

      int amount = amountStr.toInt();

      if (increase) {
        cards[cardIndex].balance += amount;
        lcd.clear();
        lcd.print("Added:");
      } else {
        if (cards[cardIndex].balance >= amount) {
          cards[cardIndex].balance -= amount;
          lcd.clear();
          lcd.print("Reduced:");
        } else {
          lcd.clear();
          lcd.print("Insufficient");
          lcd.setCursor(0, 1);
          lcd.print("funds");
          delay(2000);
          return;
        }
      }

      lcd.setCursor(0, 1);
      lcd.print(amount);
      delay(2000);

      lcd.clear();
      lcd.print("Balance:");
      lcd.setCursor(0, 1);
      lcd.print(cards[cardIndex].balance);
      delay(2000);

    } else {
      lcd.clear();
      lcd.print("Card not found");
      delay(2000);
    }
  } else {
    lcd.clear();
    lcd.print("Operation canceled");
    delay(2000);
  }
}

void deleteCard() {
  lcd.clear();
  lcd.print("Delete card:");
  lcd.setCursor(0, 1);
  lcd.print("Scan card...");

  String cardId = scanCard();  // Сканируем карту

  if (cardId != "") {
    // Поиск карты по ID
    bool cardFound = false;
    int cardIndex = -1;
    for (int i = 0; i < cardCount; i++) {
      if (cards[i].id == cardId) {
        cardFound = true;
        cardIndex = i;
        break;
      }
    }

    if (cardFound) {
      // Удаление карты путем сдвига элементов массива
      for (int i = cardIndex; i < cardCount - 1; i++) {
        cards[i] = cards[i + 1];
      }
      cardCount--; // Уменьшаем количество карт

      lcd.clear();
      lcd.print("Card deleted:");
      lcd.setCursor(0, 1);
      lcd.print(cardId);
    } else {
      lcd.clear();
      lcd.print("Card not found");
    }
  } else {
    lcd.clear();
    lcd.print("Operation canceled");
  }

  delay(2000);
}

String scanCard() {
  while (true) {
    char customKey = customKeypad.getKey();
    
    if (customKey == 'C') {  // Прерывание операции
      return ""; // Возвращаем пустую строку, если операция отменена
    }

    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      String cardId = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        cardId += String(mfrc522.uid.uidByte[i], HEX);
      }
      mfrc522.PICC_HaltA();  // Останавливаем считывание карты
      return cardId;  // Возвращаем ID считанной карты
    }
  }
}