// Copyright (c) 2013-2014 The Pebblecoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "cryptonote_genesis_config.h"

#include "cryptonote_core/cryptonote_basic.h"
#include "cryptonote_core/cryptonote_format_utils.h"

const char *GENESIS_NONCE_STRING = NULL;
const uint64_t *GENESIS_TIMESTAMP = NULL;
const char *GENESIS_COINBASE_TX_HEX = NULL;
const char *GENESIS_BLOCK_ID_HEX = NULL;
const char *GENESIS_WORK_HASH_HEX = NULL;
const char *GENESIS_HASH_SIGNATURE_HEX = NULL;

namespace cryptonote
{
  namespace config
  {
    extern bool testnet;
  }
}

namespace cryptonote_opt
{
  const command_line::arg_descriptor<std::string> arg_genesis_nonce_string = {"genesis-nonce-string", "The string that hashes into the nonce of the genesis block", "", false};
  const command_line::arg_descriptor<uint64_t> arg_genesis_timestamp =       {"genesis-timestamp", "The timestamp of the genesis block", 0, false};
  const command_line::arg_descriptor<std::string> arg_coinbase_tx_hex =      {"genesis-coinbase-hex", "The hash of the coinbase transaction of the genesis block", "", false};
  const command_line::arg_descriptor<std::string> arg_block_id_hex =         {"genesis-id-hash-hex", "The id hash for the genesis block", "", false};
  const command_line::arg_descriptor<std::string> arg_work_hash_hex =        {"genesis-work-hash-hex", "The proof of work hash for the genesis block", "", false};
  const command_line::arg_descriptor<std::string> arg_hash_signature_hex =   {"genesis-work-hash-signature-hex", "The hash of the signature for the work hash for the genesis block", "", false};

  struct genesis_info
  {
    std::string genesis_nonce_string;
    uint64_t timestamp;
    std::string coinbase_tx_hex;
    std::string block_id_hex;
    std::string work_hash_hex;
    std::string hash_signature_hex;

    std::string get_genesis_nonce_string(std::string cmd_line)
    {
      return !cmd_line.empty() ? cmd_line : genesis_nonce_string;
    }
    uint64_t get_timestamp(uint64_t cmd_line)
    {
      return cmd_line ? cmd_line : timestamp;
    }
    std::string get_coinbase_tx_hex(std::string cmd_line)
    {
      return !cmd_line.empty() ? cmd_line : coinbase_tx_hex;
    }
    std::string get_block_id_hex(std::string cmd_line)
    {
      return !cmd_line.empty() ? cmd_line : block_id_hex;
    }
    std::string get_work_hash_hex(std::string cmd_line)
    {
      return !cmd_line.empty() ? cmd_line : work_hash_hex;
    }
    std::string get_hash_signature_hex(std::string cmd_line)
    {
      return !cmd_line.empty() ? cmd_line : hash_signature_hex;
    }
  };

  genesis_info mainnet_info = {
    "The Times 18/Oct/2018 Trade war risks millions of jobs",
    UINT64_C(1539899692),
    "011e01ff00002101da20bc83273c5ec157cb0b5cf55ba229b9dcdcc60d29bd3ee160cc6fb0b283e2",
    "0a8f7c2478395494d475636761d0b8e276d1d5f4d7b98717f7fee728b9afd966",
    "b02ef300654aa4459a0959abe2c5a2e23cae521604d397b29b2af27bcb798ea2",
    "7434a8575b3c7cd32e6c649ee7933c9b5ee34782cd96c48af325a2b5deb6190e0413da112f1ca93bfecbc9b9022b676d97c9068c50e11e76a0271f36f7fe4c06",
  };

  genesis_info testnet_info = {
    "The Times 06/Oct/2018 U.S. General Considered Nuclear Response in Vietnam War, Cables Show",
    UINT64_C(1538882260),
    "011e01ff0000210135fc47419149a3999944f4abb5ba022b792f0f02d86afe295d6b9eb7a01ebd9e",
    "c72c922ba53c4364ab8fa7e3571163d84125874dbb70167da7c8e44d4992f8a1",
    "",
    "",
  };

  void init_options(boost::program_options::options_description& desc)
  {
    command_line::add_arg(desc, arg_genesis_nonce_string);
    command_line::add_arg(desc, arg_genesis_timestamp);
    command_line::add_arg(desc, arg_coinbase_tx_hex);
    command_line::add_arg(desc, arg_block_id_hex);
    command_line::add_arg(desc, arg_work_hash_hex);
    command_line::add_arg(desc, arg_hash_signature_hex);
  }

  std::string str_genesis_nonce_string;
  uint64_t genesis_timestamp;
  std::string str_coinbase_tx_hex;
  std::string str_block_id_hex;
  std::string str_work_hash_hex;
  std::string str_hash_signature_hex;

  bool handle_command_line(const boost::program_options::variables_map& vm)
  {
    if (!command_line::has_arg(vm, arg_genesis_nonce_string))
    {
      LOG_PRINT_RED_L0("Must provide genesis nonce string");
      return false;
    }
    if (!command_line::has_arg(vm, arg_genesis_timestamp))
    {
      LOG_PRINT_RED_L0("Must provide genesis timestamp");
      return false;
    }
    if (!command_line::has_arg(vm, arg_coinbase_tx_hex))
    {
      LOG_PRINT_RED_L0("Must provide coinbase tx hex");
      return false;
    }
    if (!command_line::has_arg(vm, arg_block_id_hex))
    {
      LOG_PRINT_RED_L0("Must provide genesis block id hash hex");
      return false;
    }

    genesis_info &p_default = cryptonote::config::testnet ? testnet_info : mainnet_info;

    str_genesis_nonce_string = p_default.get_genesis_nonce_string(command_line::get_arg(vm, arg_genesis_nonce_string));
    genesis_timestamp = p_default.get_timestamp(command_line::get_arg(vm, arg_genesis_timestamp));
    str_coinbase_tx_hex = p_default.get_coinbase_tx_hex(command_line::get_arg(vm, arg_coinbase_tx_hex));
    str_block_id_hex = p_default.get_block_id_hex(command_line::get_arg(vm, arg_block_id_hex));

    GENESIS_NONCE_STRING = str_genesis_nonce_string.c_str();
    GENESIS_TIMESTAMP = &genesis_timestamp;
    GENESIS_COINBASE_TX_HEX = str_coinbase_tx_hex.c_str();
    GENESIS_BLOCK_ID_HEX = str_block_id_hex.c_str();

    cryptonote::block bl;
    if (!cryptonote::generate_genesis_block(bl))
    {
      return false;
    }

    if (command_line::has_arg(vm, arg_work_hash_hex))
    {
      str_work_hash_hex = p_default.get_work_hash_hex(command_line::get_arg(vm, arg_work_hash_hex));
      if (!str_work_hash_hex.empty())
      {
        GENESIS_WORK_HASH_HEX = str_work_hash_hex.c_str();
      }
    }

    if (command_line::has_arg(vm, arg_hash_signature_hex))
    {
      str_hash_signature_hex = p_default.get_hash_signature_hex(command_line::get_arg(vm, arg_hash_signature_hex));
      if (!str_hash_signature_hex.empty())
      {
        GENESIS_HASH_SIGNATURE_HEX = str_hash_signature_hex.c_str();
      }
    }

    return true;
  }
}
