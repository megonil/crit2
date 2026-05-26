#pragma once
#include <iostream>
#include <llvm/Support/SourceMgr.h>

struct Src {
  static unsigned
  FromFile (llvm::SourceMgr &mgr, std::string_view filepath) {
    auto buff = llvm::MemoryBuffer::getFile (filepath);
    if (std::error_code e = buff.getError ()) {
      std::cerr << "SRC Error: " << e.message ();
      exit (1);
    }

    unsigned buff_id
            = mgr.AddNewSourceBuffer (std::move (*buff), llvm::SMLoc ());

    return buff_id;
  }
};
