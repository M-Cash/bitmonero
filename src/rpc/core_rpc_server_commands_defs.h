// Copyright (c) 2014-2016, The Monero Project
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once
#include "cryptonote_protocol/cryptonote_protocol_defs.h"
#include "cryptonote_core/cryptonote_basic.h"
#include "cryptonote_core/difficulty.h"
#include "crypto/hash.h"

namespace cryptonote
{
  //-----------------------------------------------
#define CORE_RPC_STATUS_OK   "OK"
#define CORE_RPC_STATUS_BUSY   "BUSY"
#define CORE_RPC_STATUS_NOT_MINING "NOT MINING"

#define CORE_RPC_VERSION 1

  struct COMMAND_RPC_GET_HEIGHT
  {
    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      uint64_t 	 height;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(height)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_GET_BLOCKS_FAST
  {

    struct request
    {
      std::list<crypto::hash> block_ids; //*first 10 blocks id goes sequential, next goes in pow(2,n) offset, like 2, 4, 8, 16, 32, 64 and so on, and the last one is always genesis block */
      uint64_t    start_height;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(block_ids)
        KV_SERIALIZE(start_height)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::list<block_complete_entry> blocks;
      uint64_t    start_height;
      uint64_t    current_height;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(blocks)
        KV_SERIALIZE(start_height)
        KV_SERIALIZE(current_height)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_GET_HASHES_FAST
  {

    struct request
    {
      std::list<crypto::hash> block_ids; //*first 10 blocks id goes sequential, next goes in pow(2,n) offset, like 2, 4, 8, 16, 32, 64 and so on, and the last one is always genesis block */
      uint64_t    start_height;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(block_ids)
        KV_SERIALIZE(start_height)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::list<crypto::hash> m_block_ids;
      uint64_t    start_height;
      uint64_t    current_height;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(m_block_ids)
        KV_SERIALIZE(start_height)
        KV_SERIALIZE(current_height)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  //-----------------------------------------------
  struct COMMAND_RPC_GET_TRANSACTIONS
  {
    struct request
    {
      std::list<std::string> txs_hashes;
      bool decode_as_json;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(txs_hashes)
        KV_SERIALIZE(decode_as_json)
      END_KV_SERIALIZE_MAP()
    };

    struct entry
    {
      std::string tx_hash;
      std::string as_hex;
      std::string as_json;
      bool in_pool;
      uint64_t block_height;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(tx_hash)
        KV_SERIALIZE(as_hex)
        KV_SERIALIZE(as_json)
        KV_SERIALIZE(in_pool)
        KV_SERIALIZE(block_height)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      // older compatibility stuff
      std::list<std::string> txs_as_hex;  //transactions blobs as hex (old compat)
      std::list<std::string> txs_as_json; //transactions decoded as json (old compat)

      // in both old and new
      std::list<std::string> missed_tx;   //not found transactions

      // new style
      std::vector<entry> txs;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(txs_as_hex)
        KV_SERIALIZE(txs_as_json)
        KV_SERIALIZE(txs)
        KV_SERIALIZE(missed_tx)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  //-----------------------------------------------
  struct COMMAND_RPC_IS_KEY_IMAGE_SPENT
  {
    enum STATUS {
      UNSPENT = 0,
      SPENT_IN_BLOCKCHAIN = 1,
      SPENT_IN_POOL = 2,
    };

    struct request
    {
      std::vector<std::string> key_images;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(key_images)
      END_KV_SERIALIZE_MAP()
    };


    struct response
    {
      std::vector<int> spent_status;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(spent_status)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  //-----------------------------------------------
  struct COMMAND_RPC_GET_TX_GLOBAL_OUTPUTS_INDEXES
  {
    struct request
    {
      crypto::hash txid;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE_VAL_POD_AS_BLOB(txid)
      END_KV_SERIALIZE_MAP()
    };


    struct response
    {
      std::vector<uint64_t> o_indexes;
      std::string status;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(o_indexes)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS
  {
    struct request
    {
      std::vector<uint64_t> amounts;
      uint64_t              outs_count;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(amounts)
        KV_SERIALIZE(outs_count)
      END_KV_SERIALIZE_MAP()
    };

#pragma pack (push, 1)
    struct out_entry
    {
      uint64_t global_amount_index;
      crypto::public_key out_key;
    };
#pragma pack(pop)

    struct outs_for_amount
    {
      uint64_t amount;
      std::list<out_entry> outs;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(amount)
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(outs)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::vector<outs_for_amount> outs;
      std::string status;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(outs)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_SEND_RAW_TX
  {
    struct request
    {
      std::string tx_as_hex;
      bool do_not_relay;

      request() {}
      explicit request(const transaction &);

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(tx_as_hex)
        KV_SERIALIZE(do_not_relay)
      END_KV_SERIALIZE_MAP()
    };


    struct response
    {
      std::string status;
      std::string reason;
      bool not_relayed;
      bool low_mixin;
      bool double_spend;
      bool invalid_input;
      bool invalid_output;
      bool too_big;
      bool overspend;
      bool fee_too_low;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(reason)
        KV_SERIALIZE(not_relayed)
        KV_SERIALIZE(low_mixin)
        KV_SERIALIZE(double_spend)
        KV_SERIALIZE(invalid_input)
        KV_SERIALIZE(invalid_output)
        KV_SERIALIZE(too_big)
        KV_SERIALIZE(overspend)
        KV_SERIALIZE(fee_too_low)
      END_KV_SERIALIZE_MAP()
    };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_START_MINING
  {
    struct request
    {
      std::string miner_address;
      uint64_t    threads_count;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(miner_address)
        KV_SERIALIZE(threads_count)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_GET_INFO
  {
    struct request
    {

      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      uint64_t height;
      uint64_t target_height;
      uint64_t difficulty;
      uint64_t target;
      uint64_t tx_count;
      uint64_t tx_pool_size;
      uint64_t alt_blocks_count;
      uint64_t outgoing_connections_count;
      uint64_t incoming_connections_count;
      uint64_t white_peerlist_size;
      uint64_t grey_peerlist_size;
      bool testnet;
      std::string top_block_hash;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(height)
        KV_SERIALIZE(target_height)
        KV_SERIALIZE(difficulty)
        KV_SERIALIZE(target)
        KV_SERIALIZE(tx_count)
        KV_SERIALIZE(tx_pool_size)
        KV_SERIALIZE(alt_blocks_count)
        KV_SERIALIZE(outgoing_connections_count)
        KV_SERIALIZE(incoming_connections_count)
        KV_SERIALIZE(white_peerlist_size)
        KV_SERIALIZE(grey_peerlist_size)
        KV_SERIALIZE(testnet)
        KV_SERIALIZE(top_block_hash)
      END_KV_SERIALIZE_MAP()
    };
  };

    
  //-----------------------------------------------
  struct COMMAND_RPC_STOP_MINING
  {
    struct request
    {

      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };


    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  //-----------------------------------------------
  struct COMMAND_RPC_MINING_STATUS
  {
    struct request
    {

      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };


    struct response
    {
      std::string status;
      bool active;
      uint64_t speed;
      uint32_t threads_count;
      std::string address;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(active)
        KV_SERIALIZE(speed)
        KV_SERIALIZE(threads_count)
        KV_SERIALIZE(address)
      END_KV_SERIALIZE_MAP()
    };
  };

  //-----------------------------------------------
  struct COMMAND_RPC_SAVE_BC
  {
    struct request
    {

      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };


    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  
  //
  struct COMMAND_RPC_GETBLOCKCOUNT
  {
    typedef std::list<std::string> request;

    struct response
    {
      uint64_t count;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(count)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };

  };

  struct COMMAND_RPC_GETBLOCKHASH
  {
    typedef std::vector<uint64_t> request;

    typedef std::string response;
  };


  struct COMMAND_RPC_GETBLOCKTEMPLATE
  {
    struct request
    {
      uint64_t reserve_size;       //max 255 bytes
      std::string wallet_address;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(reserve_size)
        KV_SERIALIZE(wallet_address)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      uint64_t difficulty;
      uint64_t height;
      uint64_t reserved_offset;
      std::string prev_hash;
      blobdata blocktemplate_blob;
      blobdata blockhashing_blob;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(difficulty)
        KV_SERIALIZE(height)
        KV_SERIALIZE(reserved_offset)
        KV_SERIALIZE(prev_hash)
        KV_SERIALIZE(blocktemplate_blob)
        KV_SERIALIZE(blockhashing_blob)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_SUBMITBLOCK
  {
    typedef std::vector<std::string> request;
    
    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  
  struct block_header_responce
  {
      uint8_t major_version;
      uint8_t minor_version;
      uint64_t timestamp;
      std::string prev_hash;
      uint32_t nonce;
      bool orphan_status;
      uint64_t height;
      uint64_t depth;
      std::string hash;
      difficulty_type difficulty;
      uint64_t reward;
      
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(major_version)
        KV_SERIALIZE(minor_version)
        KV_SERIALIZE(timestamp)
        KV_SERIALIZE(prev_hash)
        KV_SERIALIZE(nonce)
        KV_SERIALIZE(orphan_status)
        KV_SERIALIZE(height)
        KV_SERIALIZE(depth)
        KV_SERIALIZE(hash)
        KV_SERIALIZE(difficulty)
        KV_SERIALIZE(reward)
      END_KV_SERIALIZE_MAP()
  };

  struct COMMAND_RPC_GET_LAST_BLOCK_HEADER
  {
    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      block_header_responce block_header;
      
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(block_header)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };

  };
  
  struct COMMAND_RPC_GET_BLOCK_HEADER_BY_HASH
  {
    struct request
    {
      std::string hash;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(hash)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      block_header_responce block_header;
      
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(block_header)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };

  };

  struct COMMAND_RPC_GET_BLOCK_HEADER_BY_HEIGHT
  {
    struct request
    {
      uint64_t height;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(height)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      block_header_responce block_header;
      
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(block_header)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };

  };

  struct COMMAND_RPC_GET_BLOCK
  {
    struct request
    {
      std::string hash;
      uint64_t height;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(hash)
        KV_SERIALIZE(height)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      block_header_responce block_header;
      std::vector<std::string> tx_hashes;
      std::string blob;
      std::string json;
      
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(block_header)
        KV_SERIALIZE(tx_hashes)
        KV_SERIALIZE(status)
        KV_SERIALIZE(blob)
        KV_SERIALIZE(json)
      END_KV_SERIALIZE_MAP()
    };

  };

  struct peer {
    uint64_t id;
    uint32_t ip;
    uint16_t port;
    uint64_t last_seen;

    peer() = default;

    peer(uint64_t id, uint32_t ip, uint16_t port, uint64_t last_seen)
      : id(id), ip(ip), port(port), last_seen(last_seen)
    {}

    BEGIN_KV_SERIALIZE_MAP()
      KV_SERIALIZE(id)
      KV_SERIALIZE(ip)
      KV_SERIALIZE(port)
      KV_SERIALIZE(last_seen)
    END_KV_SERIALIZE_MAP()
  };

  struct COMMAND_RPC_GET_PEER_LIST
  {
    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      std::vector<peer> white_list;
      std::vector<peer> gray_list;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(white_list)
        KV_SERIALIZE(gray_list)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_SET_LOG_HASH_RATE
  {
    struct request
    {
      bool visible;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(visible)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_SET_LOG_LEVEL
  {
    struct request
    {
      int8_t level;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(level)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct tx_info
  {
    std::string id_hash;
    std::string tx_json; // TODO - expose this data directly
    uint64_t blob_size;
    uint64_t fee;
    std::string max_used_block_id_hash;
    uint64_t max_used_block_height;
    bool kept_by_block;
    uint64_t last_failed_height;
    std::string last_failed_id_hash;
    uint64_t receive_time;

    BEGIN_KV_SERIALIZE_MAP()
      KV_SERIALIZE(id_hash)
      KV_SERIALIZE(tx_json)
      KV_SERIALIZE(blob_size)
      KV_SERIALIZE(fee)
      KV_SERIALIZE(max_used_block_id_hash)
      KV_SERIALIZE(max_used_block_height)
      KV_SERIALIZE(kept_by_block)
      KV_SERIALIZE(last_failed_height)
      KV_SERIALIZE(last_failed_id_hash)
      KV_SERIALIZE(receive_time)
    END_KV_SERIALIZE_MAP()
  };

  struct spent_key_image_info
  {
    std::string id_hash;
    std::vector<std::string> txs_hashes;

    BEGIN_KV_SERIALIZE_MAP()
      KV_SERIALIZE(id_hash)
      KV_SERIALIZE(txs_hashes)
    END_KV_SERIALIZE_MAP()
  };

  struct COMMAND_RPC_GET_TRANSACTION_POOL
  {
    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      std::vector<tx_info> transactions;
      std::vector<spent_key_image_info> spent_key_images;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(transactions)
        KV_SERIALIZE(spent_key_images)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_GET_CONNECTIONS
  {
    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      std::list<connection_info> connections;
      
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(connections)
      END_KV_SERIALIZE_MAP()
    };

  };

  struct COMMAND_RPC_GET_BLOCK_HEADERS_RANGE
  {
    struct request
    {
      uint64_t start_height;
      uint64_t end_height;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(start_height)
        KV_SERIALIZE(end_height)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      std::vector<block_header_responce> headers;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(headers)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_STOP_DAEMON
  {
    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  
  struct COMMAND_RPC_FAST_EXIT
  {
	struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };
    
    struct response
    {
	  std::string status;
	  
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  
  struct COMMAND_RPC_OUT_PEERS
  {
	struct request
    {
	  uint64_t out_peers;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(out_peers)
      END_KV_SERIALIZE_MAP()
    };
    
    struct response
    {
	  std::string status;
	  
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
    
  struct COMMAND_RPC_START_SAVE_GRAPH
  {
	struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };
    
    struct response
    {
	  std::string status;
	  
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  
  struct COMMAND_RPC_STOP_SAVE_GRAPH
  {
	struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };
    
    struct response
    {
	  std::string status;
	  
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_HARD_FORK_INFO
  {
    struct request
    {
      uint8_t version;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(version)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      uint8_t version;
      bool enabled;
      uint32_t window;
      uint32_t votes;
      uint32_t threshold;
      uint8_t voting;
      uint32_t state;
      uint64_t earliest_height;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(version)
        KV_SERIALIZE(enabled)
        KV_SERIALIZE(window)
        KV_SERIALIZE(votes)
        KV_SERIALIZE(threshold)
        KV_SERIALIZE(voting)
        KV_SERIALIZE(state)
        KV_SERIALIZE(earliest_height)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_GETBANS
  {
    struct ban
    {
      uint32_t ip;
      uint32_t seconds;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(ip)
        KV_SERIALIZE(seconds)
      END_KV_SERIALIZE_MAP()
    };

    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      std::vector<ban> bans;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(bans)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_SETBANS
  {
    struct ban
    {
      uint32_t ip;
      bool ban;
      uint32_t seconds;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(ip)
        KV_SERIALIZE(ban)
        KV_SERIALIZE(seconds)
      END_KV_SERIALIZE_MAP()
    };

    struct request
    {
      std::vector<ban> bans;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(bans)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_FLUSH_TRANSACTION_POOL
  {
    struct request
    {
      std::list<std::string> txids;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(txids)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_GET_OUTPUT_HISTOGRAM
  {
    struct request
    {
      std::vector<uint64_t> amounts;
      uint64_t min_count;
      uint64_t max_count;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(amounts);
        KV_SERIALIZE(min_count);
        KV_SERIALIZE(max_count);
      END_KV_SERIALIZE_MAP()
    };

    struct entry
    {
      uint64_t amount;
      uint64_t instances;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(amount);
        KV_SERIALIZE(instances);
      END_KV_SERIALIZE_MAP()

      entry(uint64_t amount, uint64_t instances): amount(amount), instances(instances) {}
      entry() {}
    };

    struct response
    {
      std::string status;
      std::vector<entry> histogram;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(histogram)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_GET_VERSION
  {
    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      uint32_t version;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(version)
      END_KV_SERIALIZE_MAP()
    };
  };
}

