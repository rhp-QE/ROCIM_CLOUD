package util

import "errors"

func DecodeUint32LE(numberBytes []byte) (uint32, error) {
	if len(numberBytes) != 4 {
		return 0, errors.New("buf len is not 4")
	}

	var number uint32 = 0
	for index, value := range numberBytes {
		number |= (uint32(value) << (uint32(index) << 3))
	}
	return number, nil
}

func EncodeUint32LE(number uint32, numberBytes []byte) error {
	if len(numberBytes) != 4 {
		return errors.New("numberBytes len is not 4")
	}

	for i := 0; i < 4; i++ {
		numberBytes[i] = byte(number & 0xFF)
		number >>= 8
	}
	return nil
}
