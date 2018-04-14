#include "button_coin.h"

//
// ����������� - ���������� ������ ��� �������� ���������� ������ RoboCraft
//

    // ����������� ������
    button_coin::button_coin (int a, boolean b, int c) {
      pin = a;
      reg = b;
      wait = c;
      cur = 0;
      prev = 0;
      trigger_push = 0;
      trigger_let = 0;
      coint = 0;
      con = 0;
      discon = 0;
      pinMode(pin, INPUT_PULLUP);
    }
    
    // ������� ������
    bool button_coin::but () {
      knopka(); // �����������
      coin (); // ������� �������
      triger(); // �������
      connect_but (); // ������� ������
      discon_but (); // ���������� ������
      return cur; // ������� ��������
    }

// �����������
    bool button_coin::knopka() {
    static bool prew; // ���������� ��������� ������ !!������ ��� ������������!!
    static unsigned long prewMillis; // �������� ��� ������� ��������

      prev = cur;
      if (digitalRead(pin) == LOW) { // ���� ������ ������
        if (!prew) { // ���� ���������� �������� ������ "�� ������"
          prewMillis = millis(); // ������ �����
          prew = true; // ���������� ���������� �������� ������ � "������"
        }
        if (millis() - prewMillis > wait) { // ���� ����� ����� �������� ��������� ��������
          cur = 1; // ���������� �������� ������ "������"
        }
      }
      else { // ���� ������ �� ������
        if (millis() - prewMillis > wait) { // � ���� ����� ����� �������� ��������� ��������
          cur = false; // ����� �������� �������� ��������� ������
          prew = false; // ����� �������� ����������� ��������� ������
        }
      }
      return cur;
    };

    // �������
    void button_coin::triger() {    
      if (cur && !prev) // ���� ������ ������
        trigger_push = !trigger_push;
  
      if (!cur && prev) // ���� ������ ������
        trigger_let = !trigger_let;
    }
  
    // ������� �������
    int button_coin::coin () {
      if (cur && !prev)
        coint++;
    }
    
    // ������� ������
    void button_coin::connect_but () {
      if (cur && !prev) con = 1;
      else con = 0;
    }
    
    // ���������� ������
    void button_coin::discon_but () {
      if (!cur && prev) discon = 1;
      else discon = 0;
    }