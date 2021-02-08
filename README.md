# tp3-orga2

## TO DO:

- [x] Modo debug está habilitado siempre. Dejarlo deshabilitado al inicio. (sched.c)
- [x] Chequear la logica de la tecla y, no esta mal? (game.c)
- [x] Matar tareas cuando generan excepciones. (isr.asm)
- [ ] Chequear valores que imprime el debug mode.
- [ ] Cambiar la direccion de inicio de las pelotas. Que inicien hacia el jugador contrario.

## Desiciones tomadas

### Páginas

- La primer entrada del directorio de cada tarea/handler apunta a la tabla del kernel, no esta mapeada de cero para cada tarea.
- Tener páginas estáticas para las tareas y para los handlers.
- Tener estructura del kernel para manejar cuales de estas páginas estan ocupadas.
- Tener 12 TSSs estátitcos para estas tareas.

### Scheduler

- Las tareas nuevas se cargan cuando se dibuja para no quitarle tiempo a otra tarea.
- Tener una estructura del scheduler para seguir en que tick esta, los selectores que apuntan a las TSSs de las tareas y handlers, la tarea actual, si esta es un handler y el modo debug.
- El clock llama a una función que puede acceder a esta estructura y ver si esta en el tick 7 para dibujar y para cargar las tareas nuevas.

### Teclado

- Hay dos estructuras, definidas en game que guardan que tarea y que movimiento realiza cada usuario.
- Son actualizadas por la funcion key_pressed_handler, que es llamada la rutina de interrupcion del teclado.