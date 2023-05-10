#include "LuminanceExtraction.h"

LuminanceExtraction::LuminanceExtraction()
{
	renderingPipeline = Argent::Graphics::RenderingPipeline::CreateLuminanceExtractionPipeline();
}
