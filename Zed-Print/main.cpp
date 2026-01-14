#include "Zed_Print_Header.hpp"

int main() {
	Zed_Print zp;
	//std::println("{}", zp.clear());
	zp.clear_screen();

	uint16_t m_width = zp.get_width();
	uint16_t m_height = zp.get_height();
	zp.pass();

	yprintln(" Terminal Size: {} x {} ", m_width, m_height);

	zp.print(Label{" Zed's Print Function Header! ", Color::Code::GREEN});

	std::string m_error = " Missing SHA256 Checksum of Package ";
	zp.fail(m_error);

	m_error = " MD5Sum Checksum Differs ";
	zp.warn(m_error);

	return 0;
}
