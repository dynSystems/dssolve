#include "serialize.hpp"

/**
 * Create an Envelope for a message.
 * @param size Size of the message in bytes.
 * @param purpose purpose of the message.
 */
Envelope::Envelope(std::uint32_t size, Purpose purpose) : version(1), size(size), purpose(purpose)
{
}

/**
 * Create a Header for a packet.
 * @param size Size of the packet in bytes.
 * @param entity Type of the payload.
 */
Header::Header(std::uint32_t size, Objects entity) : version(1), size(size), obj(entity) {}

/**
 * Create an empty Packer.
 *
 * Even an empty Packer allocates storage for an Envelope.
 */
Packer::Packer() : m_types(SerializationTypes()) { reset(); }

/**
 * Make message sendable by adding its Header.
 *
 * @param purpose The purpose of the message.
 */
void Packer::tag(const Purpose& purpose)
{
	Envelope envelope(m_message.size(), purpose);
	std::memcpy(m_message.data(), &envelope, sizeof(Envelope));
}

/**
 * Reset the Packer.
 *
 * After this call, the Packer contains only space for an Envelope. Any packets
 * present prior to this call are removed from the Packer.
 */
void Packer::reset() { m_message.resize(sizeof(Envelope)); }

/**
 * Extract message.
 *
 * Returns a message containing the currently present packets. The Packer is not
 * reset.
 *
 * @param purpose The purpose of the message.
 */
std::vector<dss::byte> Packer::extract(const Purpose& purpose)
{
	tag(purpose);
	return m_message;
}

/**
 * Create a message from the current list of packets and hand it over to the
 * message passing system.
 *
 * Dispatch attaches a message Header indicating the Purpose of the message. The
 * message is than handed over to the message passing system, and the Packer
 * is reset. Thus, the message can not be retrieved from the Packer anymore.
 *
 * @param purpose The purpose of the message.
 */
void Packer::dispatch(const Purpose& purpose)
{
	tag(purpose);
	// FIXME: To be implemented: handing message to zeromq
	reset();
}

/**
 * Default constructor
 *
 * Construct an empty Unpacker object.
*/
Unpacker::Unpacker()
	: m_message(std::vector<dss::byte>()),
	  m_types(SerializationTypes()),
	  m_purpose(Purpose()),
	  m_pktObj(std::vector<Objects>()),
	  m_pktSizes(std::vector<std::uint32_t>()),
	  m_pPkt(std::vector<const dss::byte*>())
{
}

/**
 * Load \p message and construct its table of contents.
 *
 * If the Unpacker object currently is loaded with a message, the call to load
 * removes this loaded message. The removed message can not be recovered.
 *
 * @param message A binary message created by Packer.
 */
void Unpacker::load(const std::vector<dss::byte>& message)
{
	m_message = message;
	m_pktSizes.clear();
	m_pktObj.clear();
	m_pPkt.clear();

	// Every message has at least an envelope
	const dss::byte*       pMessage    = m_message.data();
	const dss::byte* const pMessageEnd = &m_message.back() + 1;

	// Read envelope
	const Envelope* const pEnvelope = reinterpret_cast<const Envelope*>(pMessage);
	if (pEnvelope->size != m_message.size())
		throw dss::serialization_error("Message size does not match envelope information.");
	if (pEnvelope->version != 1)
		throw dss::serialization_error("Version of message format not supported.");
	m_purpose = pEnvelope->purpose;
	pMessage += sizeof(Envelope);

	// Read packets
	while (pMessage != pMessageEnd) {
		const Header* const header = reinterpret_cast<const Header*>(pMessage);
		m_pktSizes.emplace_back(header->size);
		m_pktObj.emplace_back(header->obj);
		m_pPkt.emplace_back(pMessage + sizeof(Header));
		pMessage += header->size;
	}
}


/**
 * Get list of objects stored in the unpacker.
 */
std::vector<Objects> Unpacker::objects() { return m_pktObj; }

/**
 * Get the purpose of the message stored in the unpacker.
 */
Purpose Unpacker::purpose() { return m_purpose; }

/**
 * Return the payload size of the \p idx-th packet in bytes.
 *
 * This is the size of the payload without the packet's header.
 *
 * @param idx Index of a packet.
 */
std::uint32_t Unpacker::bytes(std::size_t idx) { return m_pktSizes[idx] - sizeof(Header); }

/**
 * Construct a connector between C++ types and the enumeration Objects.
 */
SerializationTypes::SerializationTypes()
	: m_type{{std::type_index(typeid(int)), Objects::INT},
			 {std::type_index(typeid(double)), Objects::DOUBLE}}
{
}

/**
 * Retrieve enumeration index of a type.
 *
 * @index A C++ type represented by its std::type_info returned by typeid().
 */
Objects SerializationTypes::operator()(const std::type_info& info) const
{
	return m_type.at(std::type_index(info));
}
