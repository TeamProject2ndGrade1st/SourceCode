#include "RenderContext.h"

namespace Argent::Graphics
{
	void RenderContext::Draw()
	{
		cmdList->IASetIndexBuffer(&iView);
		cmdList->IASetVertexBuffers(vertexStartSlot, numView, &vView);
		cmdList->IASetPrimitiveTopology(primitiveTopology);

	}
}