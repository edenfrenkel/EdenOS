#ifndef STDINT_H
#define STDINT_H
	#include <stdint.h>
#endif
#ifndef SYSTEM_H
#define SYSTEM_H
	#include <system.h>
#endif

#define CONFIG_ADDRESS	0x0CF8
#define CONFIG_DATA		0x0CFC

#define BUS_COUNT		256
#define DEVICE_COUNT 	32
#define FUNCTION_COUNT 	8

#define MULTI_FUNC_BIT (1 << 7)

enum CLASS_CODES
{
	NONE 									= 0x00,
	MASS_STORAGE_CONTROLLER					= 0x01,
	NETWORK_CONTROLLER						= 0x02,
	DISPLAY_CONTROLLER						= 0x03,
	MULTIMEDIA_CONTROLLER					= 0x04,
	MEMORY_CONTROLLER						= 0x05,
	BRIDGE_DEVICE							= 0x06,
	SIMPLE_COMM_CONTROLS					= 0x07,
	BASE_SYSTEM_PERIPHERALS					= 0x08,
	INPUT_DEVICE							= 0x09,
	DOCKING_STATIONS						= 0x0A,
	PROCESSORS								= 0x0B,
	SERIAL_BUS_CONTROLLERS					= 0x0C,
	WIRELESS_CONTROLLERS					= 0x0D,
	INTELLIGENT_IO_CONTROLLERS				= 0x0E,
	SATELLITE_COMM_CONTROLLERS				= 0x0F,
	ENCRYPT_DECRYPT_CONTROLLERS				= 0x10,
	DATA_ACQ_AND_SIGNAL_PROC_CONTROLLERS	= 0x11,
	UNDEFINED = 0xFF
};

enum BRIDGE_DEVICE_SUBCLASS
{
	HOST_BRIDGE			= 0x00,
	ISA_BRIDGE			= 0x01,
	EISA_BRIDGE			= 0x02,
	MCA_BRIDGE			= 0x03,
	PCI_TO_PCI_BRIDGE	= 0x04,
	PCMCIA_BRIDGE		= 0x05,
	NUBUS_BRIDGE		= 0x06,
	CARDBUS_BRIDGE		= 0x07,
	RACEWAY_BRIDGE		= 0x08,
	PCI_TO_PCI_BRIDGE_	= 0x09,
	INFINIBRAND_TO_PCI	= 0x0A,
	OTHER				= 0x80
};

const char *class_types[] =
{
	"",
	"MASS_STORAGE_CONTROLLER",
	"NETWORK_CONTROLLER",
	"DISPLAY_CONTROLLER",
	"MULTIMEDIA_CONTROLLER",
	"MEMORY_CONTROLLER",
	"BRIDGE_DEVICE",
	"SIMPLE_COMM_CONTROLS",
	"BASE_SYSTEM_PERIPHERALS",
	"INPUT_DEVICE",
	"DOCKING_STATIONS",
	"PROCESSORS",
	"SERIAL_BUS_CONTROLLERS",
	"WIRELESS_CONTROLLERS",
	"INTELLIGENT_IO_CONTROLLERS",
	"SATELLITE_COMM_CONTROLLERS",
	"ENCRYPT_DECRYPT_CONTROLLERS",
	"DATA_ACQ_AND_SIGNAL_PROC_CONTROLLERS"
};

typedef struct cfg_addr
{
	uint32_t zero		: 2;
	uint32_t reg_num	: 6;
	uint32_t func_num	: 3;
	uint32_t dev_num	: 5;
	uint32_t bus_num	: 8;
	uint32_t reserved	: 7;
	uint32_t enable		: 1;
} __attribute__((packed)) ConfigAddr;

// FUNCTION DECLARATIONS
void pci_cfg_write_b(uint8_t bus_num, uint8_t dev_num, uint8_t func_num, uint8_t reg_num, uint8_t val);
void pci_cfg_write_w(uint8_t bus_num, uint8_t dev_num, uint8_t func_num, uint8_t reg_num, uint16_t val);
void pci_cfg_write_l(uint8_t bus_num, uint8_t dev_num, uint8_t func_num, uint8_t reg_num, uint32_t val);
uint32_t pci_cfg_read(uint8_t bus_num, uint8_t dev_num, uint8_t func_num, uint8_t reg_num);
uint16_t get_vendor_id(uint8_t bus, uint8_t device, uint8_t function);
uint16_t get_device_id(uint8_t bus, uint8_t device, uint8_t function);
uint8_t get_class_code(uint8_t bus, uint8_t device, uint8_t function);
uint8_t get_subclass(uint8_t bus, uint8_t device, uint8_t function);
uint8_t get_prog_if(uint8_t bus, uint8_t device, uint8_t function);
uint8_t get_header_type(uint8_t bus, uint8_t device, uint8_t function);
uint8_t get_secondary_bus(uint8_t bus, uint8_t device, uint8_t function);
void check_bus(uint8_t bus);
void check_device(uint8_t bus, uint8_t device);
void check_function(uint8_t bus, uint8_t device, uint8_t function);
void check_all_buses(void);
void print_enum_dev(void);

int print = 0;

void pci_cfg_write_b(uint8_t bus_num, uint8_t dev_num, uint8_t func_num, uint8_t reg_num, uint8_t val)
{
	ConfigAddr cfg;
	
	cfg.zero = 0;
	cfg.reg_num = (reg_num & 0xFC) >> 2;
	cfg.func_num = func_num;
	cfg.dev_num = dev_num;
	cfg.bus_num = bus_num;
	cfg.reserved = 0;
	cfg.enable = 1;
	
	OUTL(CONFIG_ADDRESS, *((uint32_t *) &cfg));
	OUTB(CONFIG_DATA + (reg_num & 0x03), val);
}

void pci_cfg_write_w(uint8_t bus_num, uint8_t dev_num, uint8_t func_num, uint8_t reg_num, uint16_t val)
{
	ConfigAddr cfg;
	
	cfg.zero = 0;
	cfg.reg_num = (reg_num & 0xFC) >> 2;
	cfg.func_num = func_num;
	cfg.dev_num = dev_num;
	cfg.bus_num = bus_num;
	cfg.reserved = 0;
	cfg.enable = 1;
	
	OUTL(CONFIG_ADDRESS, *((uint32_t *) &cfg));
	OUTW(CONFIG_DATA + (reg_num & 0x02), val);
}

void pci_cfg_write_l(uint8_t bus_num, uint8_t dev_num, uint8_t func_num, uint8_t reg_num, uint32_t val)
{
	ConfigAddr cfg;
	
	cfg.zero = 0;
	cfg.reg_num = reg_num >> 2;
	cfg.func_num = func_num;
	cfg.dev_num = dev_num;
	cfg.bus_num = bus_num;
	cfg.reserved = 0;
	cfg.enable = 1;
	
	OUTL(CONFIG_ADDRESS, *((uint32_t *) &cfg));
	OUTL(CONFIG_DATA, val);
}

uint32_t pci_cfg_read(uint8_t bus_num, uint8_t dev_num, uint8_t func_num, uint8_t reg_num)
{
	ConfigAddr cfg;
	uint32_t data;
	
	cfg.zero = 0;
	cfg.reg_num = reg_num >> 2;
	cfg.func_num = func_num;
	cfg.dev_num = dev_num;
	cfg.bus_num = bus_num;
	cfg.reserved = 0;
	cfg.enable = 1;
	
	OUTL(CONFIG_ADDRESS, *((uint32_t *) &cfg));
	INL(data, CONFIG_DATA);
	return data;
}

uint16_t get_vendor_id(uint8_t bus, uint8_t device, uint8_t function)
{
	return (uint16_t) (pci_cfg_read(bus, device, function, 0)  & 0xFFFF);
}

uint16_t get_device_id(uint8_t bus, uint8_t device, uint8_t function)
{
	return (uint16_t) ((pci_cfg_read(bus, device, function, 0) >> 16)  & 0xFFFF);
}

uint8_t get_class_code(uint8_t bus, uint8_t device, uint8_t function)
{
	return (uint8_t) ((pci_cfg_read(bus, device, function, 0x08) >> 24) & 0xFF);
}

uint8_t get_subclass(uint8_t bus, uint8_t device, uint8_t function)
{
	return (uint8_t) ((pci_cfg_read(bus, device, function, 0x08) >> 16) & 0xFF);
}

uint8_t get_prog_if(uint8_t bus, uint8_t device, uint8_t function)
{
	return (uint8_t) ((pci_cfg_read(bus, device, function, 0x08) >> 8) & 0xFF);
}

uint8_t get_header_type(uint8_t bus, uint8_t device, uint8_t function)
{
	return (uint8_t) ((pci_cfg_read(bus, device, function, 0x0C) >> 16) & 0xFF);
}

uint8_t get_secondary_bus(uint8_t bus, uint8_t device, uint8_t function)
{
	return (uint8_t) ((pci_cfg_read(bus, device, function, 0x18) >> 8) & 0xFF);
}

void check_bus(uint8_t bus)
{
	uint8_t device;
	
	for (device = 0; device < DEVICE_COUNT; device++)
		check_device(bus, device);
}

void check_device(uint8_t bus, uint8_t device)
{
	uint8_t function, header_type;
	uint16_t vendor_id;
	
	function = 0;
	vendor_id = get_vendor_id(bus, device, function);
	if (vendor_id == 0xFFFF)
		return;
	check_function(bus, device, function);
	header_type = get_header_type(bus, device, function);
	if (header_type & MULTI_FUNC_BIT)
		for (function = 1; function < FUNCTION_COUNT; function++)
			if (get_vendor_id(bus, device, function) != 0xFFFF)
				check_function(bus, device, function);
}

void check_function(uint8_t bus, uint8_t device, uint8_t function)
{
	uint8_t class_code, sub_class, secondary_bus;
	char *buff;
	
	class_code = get_class_code(bus, device, function);
	sub_class = get_subclass(bus, device, function);
	if (print)
	{
		if (class_code && class_code <= 0x11)
		{
			buff = malloc (10);
			puts(class_types[class_code]);
			puts(" VID=0x");
			puts(uitoa(get_vendor_id(bus, device, function), buff, BASE16));
			puts(" DID=0x");
			puts(uitoa(get_device_id(bus, device, function), buff, BASE16));
			puts(" CC=0x");
			puts(uitoa(get_class_code(bus, device, function), buff, BASE16));
			puts(" SC=0x");
			puts(uitoa(get_subclass(bus, device, function), buff, BASE16));
			puts(" PI=0x");
			puts(uitoa(get_prog_if(bus, device, function), buff, BASE16));
			putc('\n');
			free(buff);
		}
	}
	switch (class_code)
	{
		case BRIDGE_DEVICE:
			if (sub_class == PCI_TO_PCI_BRIDGE)
			{
				secondary_bus = get_secondary_bus(bus, device, function);
				check_bus(secondary_bus);
			}
			break;
		case SERIAL_BUS_CONTROLLERS:
			if (sub_class == 0x03)
				config_usb(bus, device, function);
			break;
	}
}

void check_all_buses(void)
{
	uint8_t bus, function;
	uint8_t header_type;
	
	header_type = get_header_type(0, 0, 0);
	if (!(header_type & MULTI_FUNC_BIT))
		check_bus(0);
	else
	{
		for (function = 0; function <= FUNCTION_COUNT; function++)
			if (get_vendor_id(0, 0, function) == 0xFFFF)
				break;
			bus = function;
			check_bus(bus);
	}
}

void print_enum_dev(void)
{
	print = 1;
	check_all_buses();
	print = 0;
}