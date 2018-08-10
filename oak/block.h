/**
 * Copyright Oak Team 2018
 *
 * @file block.h
 * Block�ඨ��.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef OAK_BLOCK_H
#define OAK_BLOCK_H

#include <oak/block_detail.h>

namespace oak {

	/** ģ��.
	 * ����ģ��Ļ�����.
	 */
	class Block
	{
	public:
		Block();
		virtual ~Block();

	public:
		// ����.
		virtual int work() = 0;

	public:
		// ��ȡ����ǩ��.
		SignatureList inputSignatures();
		
		// ��ȡ���ǩ��.
		SignatureList outputSignatures();

	protected:
		SignatureList m_inputSigs, m_outputSigs;

	};

} // namespace oak

#endif //OAK_BLOCK_H

