/*
 *  Copyright (C) 2015  Austin Selick-Bottos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>

#include <kernel/stdio.h>
#include <kernel/error.h>
#include <kernel/acpi.h>

// "RSD PTR " in ASCII hex
#define RSDP_SIG_HI 0x20445352
#define RSDP_SIG_LO 0x20525450

// System Descrption Table Header Signatures
//
// APIC Multiple APIC Description Table
// BERT Boot Error Record Table
// BGRT Boot Graphics Resource Table
// CPEP Corrected Platform Error Polling Table
// DSDT Differentiated System Description Table
// 
// ECDT Embedded Controller Boot Resources Table
// EINJ Error Injection Table
// ERST Error Record Serialization Table
// ACP  Fixed ACPI Description Table (FADT)
// FACS Firmware ACPI Control Structure
// FPDT Firmware Performance Data Table
// GTDT Generic Timer Description Table
// HEST Hardware Error Source Table
// MSCT Maximum System Characteristics Table
// MPST Memory Power State Table
// OEMx OEM Specific Information Tables OEM Specific tables
// PCCT Platform Communications Channel Table
// PMTT Platform Memory Topology Table
// PSDT Persistent System Description Table
// RASF ACPI RAS FeatureTable
// RSDT Root System Description Table
// SBST Smart Battery Specification Table
// SLIT System Locality Distance Information Table
// SRAT System Resource Affinity Table
// SSDT Secondary System Description Table
// XSDT Extended System Description Table

// root system description pointer
typedef struct {
	// same as rsdp_t
	char signature[8];		// "RSD PTR "
	uint8_t checksum;
	char oem_id[6];
	uint8_t revision;		// 0=v1.0, 1=v2.0, etc
	uint32_t rsdt_address;
	
	// extended in acpi v2.0
	uint32_t length;		// in bytes, including header
	uint64_t xsdt_address;	// physical address
	uint8_t ext_checksum;
	uint8_t reserved[3];
} __attribute__((packed)) rsdp_t;

// common system description table header
typedef struct {
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oem_id[6];
	char oem_tableid[8];
	uint32_t oem_revision;
	uint32_t creator_id;
	uint32_t creator_revision;
} __attribute__((packed)) sdt_header_t;

// root system description table
typedef struct {
	sdt_header_t header;	// with signature="RSDT"
	uint32_t sdt0_ptr;		// pointer to first SDT
							//   # of SDTs =(header.length-sizeof(header))/4
} __attribute__((packed)) rsdt_t;

// extended system description table
typedef struct {
	sdt_header_t header;	// with signature="XSDT"
	uint64_t sdt0_ptr;		// pointer to first SDT (64bit ptr)
							//    # of SDTs =(header.length-sizeof(header))/8
} __attribute__((packed)) xsdt_t;


void acpi_init()
{
	// search for rsdp
	//   rsdp is in the first 1KB of EBDA on 16 byte boundaries (check 0x00080000 -> 0x000DFFFF)
	//   or in 0x000E0000 to 0x000FFFFF in the BIOS area
	
	uint32_t i;
	rsdp_t* rsdp = 0;
	
	for(i=0x00080000; i<0x000FFFFF; i+=0x10)
	{
		if(*((uint32_t*)i)==RSDP_SIG_HI && *((uint32_t*)(i+0x04))==RSDP_SIG_LO)
		{
			rsdp = (rsdp_t*)i;
			break;
		}
	}
	if(rsdp == 0)
	{
		kwarn("ACPI not supported!\n");
		return;
	}
	
	// check acpi version
	/*printf("ACPI v%u.0 by %c%c%c%c%c%c\n",
		rsdp->revision+1,
		rsdp->oem_id[0],rsdp->oem_id[1],
		rsdp->oem_id[2], rsdp->oem_id[4],
		rsdp->oem_id[4], rsdp->oem_id[5]);*/
	
	// validate rsdp checksum
	uint32_t sum = 0;
	for(i=0; i<20; i++) // original acpi v1.0 rsdp size is 20 bytes
	{
		sum += *(((uint8_t*)rsdp)+i);
	}
	if((uint8_t)sum != 0x00)
	{
		kerror("ACPI RSDP checksum failed!\n");
		return;
	}
	// validate the rsdp extended checksum if necessary
	sum = 0;
	if(rsdp->revision >= 1)
	{
		for(i=0; i<sizeof(rsdp_t); i++)
		{
			sum += *(((uint8_t*)rsdp)+i);
		}
		if((uint8_t)sum != 0x00)
		{
			kerror("ACPI RSDP extended checksum failed!\n");
			return;
		}
	}
	
	// parse rsdt
	//rsdt_t* rsdt = (rsdt_t*)(rsdp->rsdt_address);
}
