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
		// 构造函数.
		Block();

		/** 构造函数.
		 * @param inputSigs 输入签名列表.
		 * @param outputSigs 输出签名列表.
		 */
		Block(SignatureList inputSigs, SignatureList outputSigs);

		virtual ~Block();

	public:
		/** 执行处理操作.
		 * 对输入、输出缓冲区进行操作，更新缓冲区数据.
		 * @param inputs 输入缓冲区.作为输入/输出参数，执行完后的count字段表明实际使用的数据量.
		 * @param outputs 输出缓冲区.作为输入/输出参数，执行完后的count字段表明实际产生的数据量.
		 * @return 处理结果状态.
		 */
		virtual int work(vector_raw_data * inputs, vector_raw_data * outputs) { return WorkResult::Error; }

		// 重置模块状态.
		virtual void reset() {}

	public:
		// 获取输入签名列表.
		virtual SignatureList inputSignatures();
		
		/** 获取输出签名.
		 * 如果指定端口不存在，返回签名 signature.isValid() == false.
		 */
		Signature inputSignature(unsigned int index);

		// 获取输出签名列表.
		virtual SignatureList outputSignatures();
		
		/** 获取输出签名.
		 *如果指定端口不存在，返回签名 signature.isValid() == false.
		 */
		Signature outputSignature(unsigned int index);

	protected:

		// 检查输入输出数据是否与签名相匹配.
		bool check(vector_raw_data * inputs, vector_raw_data * outputs);

	protected:
		SignatureList m_inputSigs, m_outputSigs;

	};

} // namespace oak

#endif //OAK_BLOCK_H

