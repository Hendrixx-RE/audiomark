# Tools
CC = riscv64-elf-gcc
SIM = qemu-riscv64-static

# Flags for GCC 15 / RV64GCV
CFLAGS = -march=rv64gcv -mabi=lp64d -O3 -static

# Targets
TARGET = verify.elf
SRC = soln.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	$(SIM) -cpu rv64,v=true,vlen=128 ./$(TARGET)

clean:
	rm -f $(TARGET)
