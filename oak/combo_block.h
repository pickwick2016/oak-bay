#pragma once

#include <vector>
#include <map>
#include <memory>

#include <oak/block.h>

namespace oak {

	class BlockRuntime;

	// ����ģ��.
	class ComboBlock : public Block
	{
	public:
		ComboBlock();
		ComboBlock(SignatureList inputSigs, SignatureList outputSigs);
		virtual ~ComboBlock();

	public:
		// ���� Block

		virtual int work(vector_raw_data * inputs, vector_raw_data * outputs);
		virtual void reset();

	public:
		// ��������˿�.
		bool setInput(unsigned int index, Port port);
		
		// ��������˿�.
		bool setOutput(unsigned int index, Port port);

	public:
		// ���ģ��(�ӹ�)
		Block * add(Block * block);

		// ɾ��ģ��(����ģ��)
		void remove(Block * block);

		// �Ƿ����ģ��
		bool contain(Block * block);

		// ���а�����ģ��.
		std::vector<Block *> blocks() const;

		// ��������ģ������.
		unsigned int count() const;

	public:
		// ����ģ��.
		bool connect(Port source, Port dest);

		// �Ͽ����ӣ�ɾ�����ӹ�ϵ��.
		void disconnect(Port source, Port dest);

		// �Ƿ����ָ�����ӹ�ϵ.
		bool contain(Port source, Port dest);

		// ��ȡ���ӹ�ϵ.
		std::vector<std::pair<Port, Port>> connections() const;

		/** �������ͼ�Ƿ���Ǣ.
		 * 1����Ҫ�����Ƿ�������.
		 */
		bool checkMap();
		
		/** �ӿ��Ƿ�ƥ��.
		 * 1�����������Ƿ�ƥ��.
		 */
		bool allowConnect(Port source, Port dest);

		/** �жϽ�β�˿��Ƿ�ռ��.
		 * Ŀ��˿�ֻ��ʹ��һ��.
		 */
		bool isEndPortUsed(Port port);

		std::vector<Port> getEndPorts(Port port);
		Port getStartPort(Port port);

		// ������ͼ���л�����ƽ��.
		std::vector<Block *> flatten();

	private:
		std::shared_ptr<Block> makePlackholder(Block * block);

	private:
		std::vector<std::shared_ptr<Block>> m_blocks; // ģ��.
		std::vector<std::pair<Port, Port>> m_connections; // ���ӹ�ϵ.

		std::shared_ptr<BlockRuntime> m_runtime;

	};

} // namespace oak
