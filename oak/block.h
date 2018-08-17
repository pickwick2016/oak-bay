/**
 * Copyright Oak Team 2018
 *
 * @file block.h
 * Block类定义.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef OAK_BLOCK_H
#define OAK_BLOCK_H

#include <oak/block_detail.h>

namespace oak {

	/** 模块.
	 * 处理模块的基本类.
	 */
	class Block
	{
	public:
		Block();
		Block(SignatureList inputSigs, SignatureList outputSigs);
		virtual ~Block();

	public:
		/** 执行处理操作.
		 * 对输入、输出缓冲区进行操作，更新缓冲区数据.
		 * @param inputs 输入缓冲区.
		 * @param outputs 输出缓冲区.
		 * @return 处理结果状态.
		 */
		virtual int work(vector_raw_data * inputs, vector_raw_data * outputs) { return WorkResult::Error; }

		// 重置模块状态.
		virtual void reset() {}

	public:
		// 获取输入签名.
		virtual SignatureList inputSignatures();
		Signature inputSignature(unsigned int index);

		// 获取输出签名.
		virtual SignatureList outputSignatures();
		Signature outputSignature(unsigned int index);

	protected:

		// 检查输入输出数据是否与签名相匹配.
		bool check(vector_raw_data * inputs, vector_raw_data * outputs);

	protected:
		SignatureList m_inputSigs, m_outputSigs;

	};

} // namespace oak

#endif //OAK_BLOCK_H

