#ifndef ROW_KEY_ENCODER
#define ROW_KEY_ENCODER

#include "core-cube/kv/abstract_row_key_encoder.hpp"
#include "core-cube/cuboid/cuboid.hpp"
#include "core-cube/kv/row_key_constants.hpp"
#include "core-metadata/metadata/dimension_encoding.hpp"
#include "core-metadata/metadata/integer_dim_enc.hpp"

#include <vector>
#include <string>
#include <map>

namespace husky {
namespace cube {

class RowKeyEncoder: public AbstractRowKeyEncoder
{
public:
	RowKeyEncoder(cubeDesc cube_desc, Cuboid * cuboid);
	~RowKeyEncoder() {}
	inline int get_header_length() const {
		return hearder_length_;
	}
	inline int get_bytes_length() const {
		return get_header_length() + body_length_;
	}
	inline void fill_header(std::vector<unsigned char> & bytes) {
		int offset = 0; // for shard id(future use)
		std::vector<unsigned char> cuboid_id_bytes = cuboid_.get_bytes();
		bytes.insert(bytes.begin(), cuboid_id_bytes.begin(), cuboid_id_bytes.begin() + RowKeyConstants::ROWKEY_CUBOIDID_LEN);
	}

	/* override */
	// inline std::vector<unsigned char> creat_buf() {
	// 	std::vector<unsigned char> v;
	// 	v.reserve(get_bytes_length());
	// 	return v;
	// }
	void encode(const std::vector<unsigned char> & body_bytes, std::vector<unsigned char> & output_buf);
	std::vector<unsigned char> encode(std::map<TblColRef *, std::string> & value_map);
	std::vector<unsigned char> encode(std::vector<std::string> & values);

private:
	int body_length_ = 0;
	// int uhc_off_set_ = -1; // it's a offset to the beginning of body (for shard id)
	// int uhc_length_ = -1;
	int hearder_length_;

	void fill_column_value(TblColRef * column, int column_len, std::string & value_str, std::vector<unsigned char> & output_value, int output_offset) {
		DimensionEncoding * dimEnc = new IntegerDimEnc(4); // hard code! Should get DimEnc by column.
		return dimEnc.encode(value_str, output, output_offset);
	}
};

}  // namespace cube
}  // namespace husky

#endif