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
		virtual ~Block();

	public:
		// 处理.
		virtual int work() = 0;

	public:
		// 获取输入签名.
		SignatureList inputSignatures();
		
		// 获取输出签名.
		SignatureList outputSignatures();

	protected:
		SignatureList m_inputSigs, m_outputSigs;

	};

} // namespace oak

#endif //OAK_BLOCK_H

