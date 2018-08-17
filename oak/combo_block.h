#pragma once

#include <vector>
#include <map>
#include <memory>

#include <oak/block.h>

namespace oak {

	class BlockRuntime;

	// 复合模块.
	class ComboBlock : public Block
	{
	public:
		ComboBlock();
		ComboBlock(SignatureList inputSigs, SignatureList outputSigs);
		virtual ~ComboBlock();

	public:
		// 重载 Block

		virtual int work(vector_raw_data * inputs, vector_raw_data * outputs);
		virtual void reset();

	public:
		// 设置输入端口.
		bool setInput(unsigned int index, Port port);
		
		// 设置输出端口.
		bool setOutput(unsigned int index, Port port);

	public:
		// 添加模块(接管)
		Block * add(Block * block);

		// 删除模块(销毁模块)
		void remove(Block * block);

		// 是否包含模块
		bool contain(Block * block);

		// 所有包含的模块.
		std::vector<Block *> blocks() const;

		// 所包含的模块数量.
		unsigned int count() const;

	public:
		// 连接模块.
		bool connect(Port source, Port dest);

		// 断开连接（删除连接关系）.
		void disconnect(Port source, Port dest);

		// 是否包含指定连接关系.
		bool contain(Port source, Port dest);

		// 获取连接关系.
		std::vector<std::pair<Port, Port>> connections() const;

		/** 检查连接图是否自洽.
		 * 1、必要连接是否连接上.
		 */
		bool checkMap();
		
		/** 接口是否匹配.
		 * 1、连接类型是否匹配.
		 */
		bool allowConnect(Port source, Port dest);

		/** 判断结尾端口是否占用.
		 * 目标端口只能使用一次.
		 */
		bool isEndPortUsed(Port port);

		std::vector<Port> getEndPorts(Port port);
		Port getStartPort(Port port);

		// 将流程图队列化（扁平）.
		std::vector<Block *> flatten();

	private:
		std::shared_ptr<Block> makePlackholder(Block * block);

	private:
		std::vector<std::shared_ptr<Block>> m_blocks; // 模块.
		std::vector<std::pair<Port, Port>> m_connections; // 连接关系.

		std::shared_ptr<BlockRuntime> m_runtime;

	};

} // namespace oak
