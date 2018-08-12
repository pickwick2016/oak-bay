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
		/** ִ�д������.
		 * �����롢������������в��������»���������.
		 * @param inputs ���뻺����.
		 * @param outputs ���������.
		 * @return ������״̬.
		 */
		virtual int work(vector_raw_data & inputs, vector_raw_data & outputs) = 0;

		// ����ģ��״̬.
		virtual void reset() {}

	public:
		// ��ȡ����ǩ��.
		SignatureList inputSignatures();
		Signature inputSignature(unsigned int index);

		// ��ȡ���ǩ��.
		SignatureList outputSignatures();
		Signature outputSignature(unsigned int index);

	protected:
		SignatureList m_inputSigs, m_outputSigs;

	};

} // namespace oak

#endif //OAK_BLOCK_H

