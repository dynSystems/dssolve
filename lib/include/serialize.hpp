/**
 * @file
 * @brief Pack and unpack messages for the message passing system, zeromq.
 *
 * Pack and unpack payload in binary packets and messages to be send via a
 * message passing system to and from the dss kernel. A Packer object
 * is used to transform std containers in binary messages for sending, while
 * Unpacker extracts data in from messages into std containers.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include "exceptions.hpp"

namespace dss {
using byte = uint8_t;
}

/**
 * Types registered for serialization.
 *
 * This enumeration is the ontology of types registered for serialization. It is
 * used by SerializationTypes to indicate the type of packet.
 *
 * In order to register new type/class for serialization add it to this enum.
 * Data is serialized by simply reading its binary representation through a
 * pointer of type byte.
 */
enum class Objects : uint8_t
{
	INT    = 1,
	DOUBLE = 2
};

/**
 * Comparison operator for scoped enum Type.
 *
 * Instances of this enum are compared via their values casted to the
 * underlying type of the enum, uint8_t.
 */
inline bool operator==(Objects lhs, Objects rhs)
{
	return static_cast<uint8_t>(lhs) == static_cast<uint8_t>(rhs);
}

/**
 * Purpose of a message.
 *
 * The Envelope on a message indicates the purpose of the message. This is the
 * ontology of purposes.
 */
enum class Purpose : uint8_t
{
	PRINT = 1
};

/**
 * Description of an entire message.
 *
 * Every message has an envelope which informs about the Purpose of the message,
 * its size in bytes as well as the version of the header itself.
 */
struct Envelope
{
	uint8_t       version;
	std::uint32_t size;
	Purpose       purpose;

	Envelope(std::uint32_t size, Purpose purpose);
};

/**
 * Description of a single packet.
 *
 * A message contains zero or more packets. each packet has a header informing
 * about the size of the packet in bytes, the type (int, double, ...) of its
 * serialized payload as well as the version of the header itself.
 */
struct Header
{
	uint8_t       version;
	std::uint32_t size;
	Objects        obj;

	Header(std::uint32_t size, Objects obj);
};

/**
 * Connect C++ types to the enumeration Objects.
 *
 * In order to keep track of types once payload is serialized, serializable
 * types are recorded in the enumeration Objects. This class allows to get the
 * enumeration index linked to a C++ type.
 */
class SerializationTypes
{
  private:
	const std::unordered_map<std::type_index, Objects> m_type;

  public:
	SerializationTypes();

		Objects operator()(const std::type_info& info) const;
};

/**
 * Create packets from std vectors.
 *
 * Binary packets are created from std vectors and stacked to form a message.
 * Each packet has its own Header, while the message has one Envelope.
 */
class Packer
{
  private:
	std::vector<dss::byte>        m_message;
	const SerializationTypes m_types;

	template <typename F>
	void append(Objects obj, const std::vector<F>& payload);

	void tag(const Purpose& purpose);

  public:
	explicit Packer();

	template <typename F>
	void append(const std::vector<F>& payload);

	std::vector<dss::byte> extract(const Purpose& purpose);

	void dispatch(const Purpose& purpose);

	void reset();
};

/**
 * Unpack a binary packet into a std container.
 */
class Unpacker
{
  private:
	std::vector<dss::byte>  m_message;
	SerializationTypes m_types;
	Purpose            m_purpose;

	std::vector<Objects>        m_pktObj;
	std::vector<std::uint32_t> m_pktSizes;
	std::vector<const dss::byte*>   m_pPkt;

	std::uint32_t bytes(std::size_t idx);

	template <typename F>
	std::uint32_t size(std::size_t idx);

	template <typename F>
	bool typeMatch(std::size_t idx);

  public:
	Unpacker();

	void load(const std::vector<dss::byte>& message);

	std::vector<Objects> objects();

	Purpose purpose();

	template <typename F>
	std::vector<F> get(std::size_t idx);
};

// Implementation of templates

template <typename F>
void Packer::append(Objects obj, const std::vector<F>& payload)
{
	auto bytesHeader  = static_cast<dss::byte>(sizeof(Header));
	auto bytesPayload = static_cast<dss::byte>(sizeof(F) * payload.size());
	auto bytesPacket  = bytesHeader + bytesPayload;

	auto currentSize = m_message.size();
	m_message.resize(m_message.size() + bytesPacket);
	auto   pPosNewPacket = m_message.data() + currentSize;
	Header header(bytesPacket, obj);
	std::memcpy(pPosNewPacket, &header, bytesHeader);

	pPosNewPacket += bytesHeader;
	std::memcpy(pPosNewPacket, payload.data(), bytesPayload);
}

/**
 * Add a packet.
 *
 * The content of \p payload is serialized and placed in a packet. The packet
 * is appended to the current packet list. \p payload must not be of size zero.
 *
 * @param payload Data to be send by the packet.
 */
template <typename F>
void Packer::append(const std::vector<F>& payload)
{
	if (payload.size() > 0) {
		try {
			// Figure out the SerializationType of payload
			append(m_types(typeid(payload[0])), payload);
		} catch (std::out_of_range) {
			throw dss::serialization_error("Type not registered for serialization.");
		}
	} else {
		throw dss::serialization_error("Can't place an empty packet.\n");
	}
}

template <typename F>
bool Unpacker::typeMatch(std::size_t idx)
{
	return m_types(typeid(F)) == m_pktObj[idx];
}

template <typename F>
std::uint32_t Unpacker::size(std::size_t idx)
{
	if (typeMatch<F>(idx))
		return bytes(idx) / sizeof(F);
	else
		throw dss::serialization_error("Type missmatch: Can't calculate number of elements.");
}

/** Get the payload of packet \p idx as std::vector\<F\>.
 *
 * The functions types() informs about the type (int, double, ...) of the
 * packet in \p message at position \p idx. Use this information to extract
 * the packet into a std::vector of correct type. The requested type is
 * checked against the type stored in the packet. In case of a type
 * miss-match, an exception dss::WhatDoWeThrowHere() is thrown.
 */
template <typename F>
std::vector<F> Unpacker::get(std::size_t idx)
{
	if (m_pPkt.size() <= idx)
		throw std::out_of_range("Packet index out of range.");

	std::vector<F> values;
	if (typeMatch<F>(idx)) {
		values.resize(size<F>(idx));
		std::memcpy(values.data(), m_pPkt[idx], bytes(idx));
	} else {
		throw dss::serialization_error("Type missmatch. Can't extract data.");
	}

	return values;
}
