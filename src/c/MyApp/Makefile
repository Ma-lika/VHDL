# La taille de la RAM, en octets.
MEM_SIZE = 65536

# La liste des fichiers sources à ajouter à l'application, en plus de MyApp.c
C_DEPS = \
	../LibC/string.c \
    ../Utilities/int_to_string.c \
	../InterruptController/InterruptController.c \
	../UART/UART-buffered.c \
    ../UART/Sonar.c \
	../Timer/Timer.c \
	../SPI/SPI.c \
	../SPI/Joystick.c \
	../SPI/Accelerometer.c \
    ../SPI/OLED.c \
    ../I2C/I2C.c \
    ../I2C/Thermometer.c \
	Platform.c

# Les options de compilation de cette application.
# Ici, minimiser la taille du programme exécutable.
C_FLAGS_USER = -Os

# Ce Makefile compilera le programme MyApp.c
# et produira un exécutable au format Intel hex.
all: MyApp.hex

include ../../../scripts/Virgule.make
