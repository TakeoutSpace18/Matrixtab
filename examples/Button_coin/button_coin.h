#ifndef button_coin_h
#define button_coin_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif 

class button_coin {
public:
    int pin; // ����� ���� ������, �������� ��� �������������
    bool reg; // ����� ������ ������, �������� ��� �������������
                  // -0- ���� ��������� ����������� ������ ��������� � �������
                  // -1- ���� ��������� ����������� ������ ��������� � ������ ����� �������� 1�
    int wait; // ����� �������� ������������
    bool cur; // ������� ��������� ������
    bool prev; // ���������� ��������� ������
    bool trigger_push; // ������� �� ������� ������
    bool trigger_let; // ������� �� ���������� ������
    int coint; // ������� �������
    bool con; // ������ ������� ������
    bool discon; // ������ ���������� ������
  
    // ����������� ������
    button_coin (int a, boolean b, int c);
    
    // ������� ������
    bool but ();

private:
	bool knopka();

	void triger();

	int coin ();

	void connect_but ();

	void discon_but ();
};

#endif

