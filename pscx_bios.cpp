#include "pscx_bios.h"

#include <fstream>
#include <iterator>
#include "stdint.h"

#include "stdlib.h"
#include "stdio.h"

#define D_DEBUG

//??
//------------------------------------------------
// TODO : to implement the loadBios function.
// It should load the BIOS binary file into array of bytes u8 (m_data).
//
// Rust:
// pub fn new(path: &Path) -> Result<Bios> {
//    let file = try!(File::open(path));
//
//    let mut data = Vec::new();
//
//    Load the BIOS
//    try!(file.take(BIOS_SIZE).read_to_end(&mut data));
//
//    if data.len() == BIOS_SIZE as usize {
//       Ok(Bios { data: data })
//    } else {
//       Err(Error::new(ErrorKind::InvalidInput,
//                      "Invalid_BIOS_size"))
//    }
//--------------------------------------------------
//??
Bios::BiosState Bios::loadBios(std::string path)
{
	// Fixme

	FILE *file;
	if((file  = fopen(path.c_str(), "rb")) == NULL)
	{
		return BIOS_STATE_INCORRECT_FILENAME;
	}



	size_t num;
	uint32_t len = m_range.m_length;
//	uint8_t tmp_bytes[10];
	uint8_t tmp_byte;
	size_t size_of_elements = 1;
	size_t num_of_elements = 1;
//	size_t num_of_elements = 2;
	uint32_t counter = 0;

//	uint8_t data[m_range.m_length] = { 0 };

	std::vector<uint8_t> data;
	m_data.assign(m_range.m_length, 0);
//	while(len >= 0)
	while(len--)
	{

		num = fread((void *)&tmp_byte, size_of_elements, num_of_elements, file);
//		uint16_t tmp_value = tmp_byte;
//		if(counter < 20)
//			printf("%X\t%X\t%X\t%X\n", tmp_value, tmp_value >> 8, tmp_value << 8, tmp_value >> 8 | tmp_value << 8);
//		tmp_byte = (uint8_t)(tmp_value >> 8 | tmp_value << 8);
//		m_data.push_back(tmp_byte);
		uint32_t pos = counter;
/*		if(pos % 2 == 0)
		{
			pos++;
		}
		else
		{
			pos--;
		} */
//		data[pos] = tmp_byte;
		m_data[pos] = tmp_byte;
		counter++;
//		m_data.push_back(tmp_bytes[1]);
//		m_data.push_back(tmp_bytes[0]);
//		len -= 2;
	}

	fclose(file);

//	m_data = data;

#ifdef D_DEBUG

	for(int i = 1; i < 31; i++)
	{
		printf("%.2X", m_data[i - 1]);
		if(i % 8 == 0)
		{
			printf("\n");
			continue;
		}
		if(i % 4 == 0)
			printf(" ");

	}

#endif

	if(true)
	{
		return BIOS_STATE_SUCCESS;
	}
	else
	{
		return BIOS_STATE_INVALID_BIOS_SIZE;
	}
}

uint32_t Bios::load32(uint32_t offset)
{
	uint32_t b0 = m_data[(size_t)offset + 0];
	uint32_t b1 = m_data[(size_t)offset + 1];
	uint32_t b2 = m_data[(size_t)offset + 2];
	uint32_t b3 = m_data[(size_t)offset + 3];

	return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}
