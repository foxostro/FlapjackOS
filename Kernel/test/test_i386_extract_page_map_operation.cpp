#include "catch.hpp"
#include <platform/i386/extract_page_map_operation.hpp>
#include "mock_paging_context.hpp"

TEST_CASE("i386::ExtractPageMapOperation", "[i386]")
{
    MockPagingContext context;
    i386::ExtractPageMapOperation operation{context.mmu_};
    auto pml2 = operation.extract();
    (void)pml2;
    // REQUIRE(false);
}
