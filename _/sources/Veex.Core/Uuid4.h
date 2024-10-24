#pragma once
#include <string>
#include <array>
#include <stdint.h>
#include "dllexp.h"

namespace Veex::Core
{
    /// <summary>
    /// Универсальный уникальный идентификатор (UUID) v4
    /// </summary>
    class Uuid4 final
    {
    public:
        /// <summary>
        /// Предоставляет новый экземпляр Uuid
        /// </summary>
        VEEX_CORE_API Uuid4();

        /// <summary>
        /// Предоставляет новый экземпляр Uuid c указанным значением
        /// </summary>
        /// <param name="value">Значение Uuid в виде массива байтов</param>
        VEEX_CORE_API Uuid4(const std::array<uint8_t, 16>& value);

        /// <summary>
        /// Предоставляет новый экземпляр Uuid, основанного на значении указанного
        /// </summary>
        /// <param name="uuid">Uuid, используемый в качестве исходного значения</param>
        VEEX_CORE_API Uuid4(const Uuid4& uuid);

        VEEX_CORE_API ~Uuid4() = default;

        /// <summary>
        /// Значение идентификатора
        /// </summary>
        std::array<uint8_t, 16> VEEX_CORE_API Value() const;

        /// <summary>
        /// Строковое представление идентификатора
        /// </summary>
        std::string_view VEEX_CORE_API ToString() const;        

        /// <summary>
        /// Равны ли указанные UUID
        /// </summary>
        bool VEEX_CORE_API operator==(const Uuid4& other);

        /// <summary>
        /// Не равны ли указанные UUID
        /// </summary>
        bool VEEX_CORE_API operator!=(const Uuid4& other);

        /// <summary>
        /// Парсинг строки, содержащей UUID
        /// </summary>
        /// <param name="uuidString">Строка, являющаяся представлением UUID v4</param>
        /// <returns>Новый экземпляр Uuid</returns>
        static Uuid4 VEEX_CORE_API  Parse(std::string_view uuidString);

    private:
        std::array<uint8_t, 8> toByteArray(uint64_t value);
        std::array<uint8_t, 16> mergeArrays(const std::array<uint8_t, 8>& high, const std::array<uint8_t, 8>& low);
        std::string stringify(const std::array<uint8_t, 16>& array);
        std::string byteToHex(uint8_t byte);

        static bool isValid(std::string_view uuidString);
        static std::array<uint8_t, 16> parse(std::string_view uuidString);

    private:
        std::array<uint8_t, 16> m_value;
        std::string m_stringValue;
    };
}
