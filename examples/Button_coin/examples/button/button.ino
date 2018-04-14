#include <button_coin.h>

// Создать экземпляр класса button
button_coin button(2, 0, 100); // параметры:
//    1-й параметр - номер пина к которому подключена кнопка (0-13)
//    2-й параметр - режим работы кнопки (0-нормально замкнутая на землю; 1-нормальна замкнутая на +5В через сопротивление 1КОм)
//    3-й параметр - время задержки дребезка в миллисекундах

void setup(void) {
  Serial.begin(9600);
}

void loop () {
  button.but(); // обработка кнопки
  Serial.print(button.cur); // текущее состояние кнопки
  Serial.print("  ");
  Serial.print(button.prev); // предыдущее состояние кнопки
  Serial.print("  ");
  Serial.print(button.trigger_push); // триггер кнопки на нажатие
  Serial.print("  ");
  Serial.print(button.trigger_let); // триггер кнопки на отпускание
  Serial.print("  ");
  Serial.print(button.coint); // счетчик нажатий кнопки с запуска библиотеки
  Serial.print("  ");
  Serial.print(button.con); // момент нажатия кнопки
  Serial.print("  ");
  Serial.println(button.discon); // момент отпускания кнопки

}
