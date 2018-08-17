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
		Block(SignatureList inputSigs, SignatureList outputSigs);
		virtual ~Block();

	public:
		/** ִ�д������.
		 * �����롢������������в��������»���������.
		 * @param inputs ���뻺����.
		 * @param outputs ���������.
		 * @return ������״̬.
		 */
		virtual int work(vector_raw_data * inputs, vector_raw_data * outputs) { return WorkResult::Error; }

		// ����ģ��״̬.
		virtual void reset() {}

	public:
		// ��ȡ����ǩ��.
		virtual SignatureList inputSignatures();
		Signature inputSignature(unsigned int index);

		// ��ȡ���ǩ��.
		virtual SignatureList outputSignatures();
		Signature outputSignature(unsigned int index);

	protected:

		// ���������������Ƿ���ǩ����ƥ��.
		bool check(vector_raw_data * inputs, vector_raw_data * outputs);

	protected:
		SignatureList m_inputSigs, m_outputSigs;

	};

} // namespace oak

#endif //OAK_BLOCK_H

