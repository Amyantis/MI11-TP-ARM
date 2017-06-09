################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fifo.c \
../imx_serial.c \
../noyau.c \
../noyaufil.c \
../pipe.c \
../prodcons_pipe.c \
../sem.c \
../serialio.c 

S_UPPER_SRCS += \
../crt0.S \
../lowlevel_init.S 

OBJS += \
./crt0.o \
./fifo.o \
./imx_serial.o \
./lowlevel_init.o \
./noyau.o \
./noyaufil.o \
./pipe.o \
./prodcons_pipe.o \
./sem.o \
./serialio.o 

C_DEPS += \
./fifo.d \
./imx_serial.d \
./noyau.d \
./noyaufil.d \
./pipe.d \
./prodcons_pipe.d \
./sem.d \
./serialio.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	arm-none-eabi-as -ggdb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -ffreestanding -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


