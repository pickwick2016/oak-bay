/**
 * Copyright Oak Team 2018
 *
 * @file flowgraph.h
 * ����ͼ����.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef OAK_FLOWGRAPH_H
#define OAK_FLOWGRAPH_H

#include <utility>
#include <vector>

#include <oak/block.h>
#include <oak/block_detail.h>

namespace oak {

	/**
	 * ����ͼ.
	 */
	class Flowgraph
	{
	public:
		void connect(Port source, Port dest);
		void disconnect(Port source, Port dest);

		std::vector<Block*> flatten();

	private:
		std::vector<std::pair<Port, Port>> m_connections;
	};

} // namespace oak

#endif //OAK_FLOWGRAPH_H
