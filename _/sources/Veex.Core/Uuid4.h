#pragma once
#include <string>
#include <array>
#include <stdint.h>
#include "dllexp.h"

/// <summary>
/// Размер UUID (в байтах)
/// </summary>
constexpr size_t UUID_SIZE = 16;

/// <summary>
/// Размер строкового представления UUID
/// </summary>
constexpr size_t UUID_STRING_SIZE = 36;

/// <summary>
/// Символ разделителя UUID (дефис)
/// </summary>
constexpr char DASH_CHAR = '-';

/// <summary>
/// Символ, определяющий версию UUID
/// </summary>
constexpr char VERSION_CHAR = '4';

/// <summary>
/// Позиции разделителей в UUID
/// </summary>
constexpr std::array<size_t, 4> UUID_DASH_POSITIONS({ 8, 13, 18, 23 });

/// <summary>
/// Набор значений для конвертации байтов в строковое hex-представление
/// </summary>
constexpr std::array<char, 16> HEX_VALUES({ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' });

/// <summary>
/// Универсальный уникальный идентификатор (UUID) v4
/// </summary>
class VEEX_CORE_API Uuid4 final
{
public:
    /// <summary>
    /// Предоставляет новый экземпляр Uuid
    /// </summary>
    Uuid4();

    /// <summary>
    /// Предоставляет новый экземпляр Uuid c указанным значением
    /// </summary>
    /// <param name="value">Значение Uuid в виде массива байтов</param>
    Uuid4(const std::array<uint8_t, 16>& value);

    /// <summary>
    /// Предоставляет новый экземпляр Uuid, основанного на значении указанного
    /// </summary>
    /// <param name="uuid">Uuid, используемый в качестве исходного значения</param>
    Uuid4(const Uuid4& uuid);

    ~Uuid4() = default;

    /// <summary>
    /// Значение идентификатора
    /// </summary>
    std::array<uint8_t, 16> Value() const;

    /// <summary>
    /// Строковое представление идентификатора
    /// </summary>
    std::string_view ToString() const;

    /// <summary>
    /// Парсинг строки, содержащей UUID
    /// </summary>
    /// <param name="uuidString">Строка, являющаяся представлением UUID v4</param>
    /// <returns>Новый экземпляр Uuid</returns>
    static Uuid4 Parse(std::string_view uuidString);

    /// <summary>
    /// Равны ли указанные UUID
    /// </summary>
    friend bool operator==(const Uuid4& lhs, const Uuid4& rhs);

    /// <summary>
    /// Не равны ли указанные UUID
    /// </summary>
    friend bool operator!=(const Uuid4& lhs, const Uuid4& rhs);    

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
