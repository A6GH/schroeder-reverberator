################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../src/SPORT1_ISR_process_samples_.asm 

C_SRCS += \
../src/Main.c \
../src/SPORT01_TDM_init.c \
../src/init1939viaSPI.c \
../src/initSRU.c \
../src/init_PLL_SDRAM.c \
../src/process_audio.c 

SRC_OBJS += \
./src/Main.doj \
./src/SPORT01_TDM_init.doj \
./src/SPORT1_ISR_process_samples_.doj \
./src/init1939viaSPI.doj \
./src/initSRU.doj \
./src/init_PLL_SDRAM.doj \
./src/process_audio.doj 

ASM_DEPS += \
./src/SPORT1_ISR_process_samples_.d 

C_DEPS += \
./src/Main.d \
./src/SPORT01_TDM_init.d \
./src/init1939viaSPI.d \
./src/initSRU.d \
./src/init_PLL_SDRAM.d \
./src/process_audio.d 


# Each subdirectory must supply rules for building sources it contributes
src/Main.doj: ../src/Main.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Schroeder_Reverberator" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG @includes-87f18efc813b34215c1088035b20b383.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/Main.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/SPORT01_TDM_init.doj: ../src/SPORT01_TDM_init.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Schroeder_Reverberator" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG @includes-87f18efc813b34215c1088035b20b383.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/SPORT01_TDM_init.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/SPORT1_ISR_process_samples_.doj: ../src/SPORT1_ISR_process_samples_.asm
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC Assembler'
	easm21k -file-attr ProjectName="Schroeder_Reverberator" -proc ADSP-21489 -si-revision 0.2 -g -DCORE0 -D_DEBUG @includes-8298c3f6f5dfdfc1c54920975249a04d.txt -swc -gnu-style-dependencies -MM -Mo "src/SPORT1_ISR_process_samples_.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/init1939viaSPI.doj: ../src/init1939viaSPI.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Schroeder_Reverberator" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG @includes-87f18efc813b34215c1088035b20b383.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/init1939viaSPI.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/initSRU.doj: ../src/initSRU.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Schroeder_Reverberator" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG @includes-87f18efc813b34215c1088035b20b383.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/initSRU.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/init_PLL_SDRAM.doj: ../src/init_PLL_SDRAM.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Schroeder_Reverberator" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG @includes-87f18efc813b34215c1088035b20b383.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/init_PLL_SDRAM.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/process_audio.doj: ../src/process_audio.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Schroeder_Reverberator" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG @includes-87f18efc813b34215c1088035b20b383.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/process_audio.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


