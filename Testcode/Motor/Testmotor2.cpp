#include <iostream>
#include <pigpiod_if2.h>
#include <unistd.h>

// Definities voor de pigpio pinnen
#define COIL_A_1_PIN 13 // 4 pink
#define COIL_A_2_PIN 26 // 17 orange
#define COIL_B_1_PIN 19 // 24 yellow
#define COIL_B_2_PIN 6  // 3 blue

int pi = pigpio_start(NULL, NULL);

class Motor {
public:
    Motor();
    ~Motor();
    void setStep(int w1, int w2, int w3, int w4);
    void forward(double delay, int steps);
    void backwards(double delay, int steps);

private:
    //int pi;
};

Motor::Motor() {


    set_mode(pi, COIL_A_1_PIN, PI_OUTPUT);
    set_mode(pi, COIL_A_2_PIN, PI_OUTPUT);
    set_mode(pi, COIL_B_1_PIN, PI_OUTPUT);
    set_mode(pi, COIL_B_2_PIN, PI_OUTPUT);
}

Motor::~Motor() {
    pigpio_stop(pi);
}

void Motor::setStep(int w1, int w2, int w3, int w4) {
    gpio_write(pi, COIL_A_1_PIN, w1);
    gpio_write(pi, COIL_A_2_PIN, w2);
    gpio_write(pi, COIL_B_1_PIN, w3);
    gpio_write(pi, COIL_B_2_PIN, w4);
}

void Motor::forward(double delay, int steps) {
    int Seq[][4] = {
        {0, 1, 0, 0},
        {0, 1, 0, 1},
        {0, 0, 0, 1},
        {1, 0, 0, 1},
        {1, 0, 0, 0},
        {1, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 1, 1, 0}};

    for (int i = 0; i < steps; ++i) {
        for (int j = 0; j < 8; ++j) {
            setStep(Seq[j][0], Seq[j][1], Seq[j][2], Seq[j][3]);
            time_sleep(delay);
        }
    }
}

void Motor::backwards(double delay, int steps) {
    int Seq[][4] = {
        {0, 1, 0, 0},
        {0, 1, 0, 1},
        {0, 0, 0, 1},
        {1, 0, 0, 1},
        {1, 0, 0, 0},
        {1, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 1, 1, 0}};

    for (int i = 0; i < steps; ++i) {
        for (int j = 7; j >= 0; --j) {
            setStep(Seq[j][0], Seq[j][1], Seq[j][2], Seq[j][3]);
            time_sleep(delay);
         }
    }
}

int main() {
    if (pi < 0) {
        std::cerr << "Failed to initialize pigpio: " << pigpio_error(pi) << >
        return 1;
    }

    Motor motor;


    while (true) {
        double delay;
        int steps;

        std::cout << "Time Delay (ms)? ";
        std::cin >> delay;

        std::cout << "How many steps forward? ";
        std::cin >> steps;
        motor.forward(delay / 1000.0, steps);

        std::cout << "How many steps backwards? ";
        std::cin >> steps;
        motor.backwards(delay / 1000.0, steps);
    }

    return 0;
}

