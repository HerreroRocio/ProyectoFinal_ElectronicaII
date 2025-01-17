#ifndef FSM_MANAGER_H
#define FSM_MANAGER_H

// Define los estados del sistema
enum SystemState { IDLE, CHECK_ENTRY, CHECK_EXIT, UPDATE_DISPLAY, RELAY_CONTROL };

// Declare the current state variable
extern SystemState currentState;

// Función para inicializar la FSM
void initFSM();

// Función para ejecutar la FSM
void runFSM();

// Declaración de funciones auxiliares (si es necesario)
void processEntry();
void processExit();

#endif // FSM_MANAGER_H
