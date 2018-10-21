// Copyright (c) 2015 The Pebblecoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <cassert>
#include <algorithm>
#include <atomic>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "include_base_utils.h"
#include "misc_language.h"

#include "blocking_udp_client.h"
#include "ntp_time.h"

using boost::asio::ip::udp;

namespace tools
{
  static const char *DEFAULT_NTP_SERVERS[] = {
    // Apple
    "time.apple.com",

    // Microsoft
    "time.windows.com",

    // Google
    "time1.google.com",
    "time2.google.com",
    "time3.google.com",
    "time4.google.com",

    // Hurricane Electric
    "clock.sjc.he.net",
    "clock.nyc.he.net",

    // Russian Federation
    "ntp.karelia.pro",
    "ntp.alpet.me",
    "aviel.alpet.me",
    "ntp.sampo.ru",
    "ntp.szt.ru",
    "ntp.ix.ru",
    "ntp1.stratum2.ru",
    "ntp2.stratum2.ru",
    "ntp3.stratum2.ru",
    "ntp4.stratum2.ru",
    "ntp5.stratum2.ru",
    "ntp1.stratum1.ru",
    "ntp2.stratum1.ru",
    "ntp3.stratum1.ru",
    "ntp4.stratum1.ru",
    "ntp5.stratum1.ru",
    "ntp1.vniiftri.ru",
    "ntp2.vniiftri.ru",
    "ntp3.vniiftri.ru",
    "ntp4.vniiftri.ru",
    "ntp21.vniiftri.ru",
    "ntp1.niiftri.irkutsk.ru",
    "ntp2.niiftri.irkutsk.ru",
    "vniiftri.khv.ru",
    "vniiftri2.khv.ru",
    "ntp0.zenon.net",
    "ntp.mobatime.ru",
    "0.ru.pool.ntp.org",
    "1.ru.pool.ntp.org",
    "2.ru.pool.ntp.org",
    "3.ru.pool.ntp.org",

    // United States
    "tock.cs.unlv.edu",
    "timex.cs.columbia.edu",
    "tick.cs.unlv.edu",
    "sundial.columbia.edu",
    "ntp-1.ece.cmu.edu",
    "ntp-2.ece.cmu.edu",
    "ntp-3.ece.cmu.edu",
    "ntp1.cs.wisc.edu",
    "ntp2.cs.wisc.edu",
    "ntp3.cs.wisc.edu",
    "ntp4.cs.wisc.edu",
    "ntp-01.caltech.edu",
    "ntp-02.caltech.edu",
    "ntp-03.caltech.edu",
    "ntp-04.caltech.edu",
    "nist0-pa.ustiming.org",
    "nist1-pa.ustiming.org",
    "nist2-pa.ustiming.org",
    "time.nist.gov",
    "time-a.nist.gov",
    "time-b.nist.gov",
    "time-c.nist.gov",
    "time-d.nist.gov",
    "time-nw.nist.gov",
    "nist1-macon.macon.ga.us",
    "nist.netservicesgroup.com",
    "wwv.nist.gov",
    "time-a.timefreq.bldrdoc.gov",
    "time-b.timefreq.bldrdoc.gov",
    "time-c.timefreq.bldrdoc.gov",
    "utcnist.colorado.edu",
    "utcnist2.colorado.edu",
    "nist1-lv.ustiming.org",
    "time-nw.nist.gov",
    "ntp1.bu.edu",
    "ntp2.bu.edu",
    "ntp3.bu.edu",
    "0.us.pool.ntp.org",
    "1.us.pool.ntp.org",
    "2.us.pool.ntp.org",
    "3.us.pool.ntp.org",
    "otc1.psu.edu",
    "otc2.psu.edu",
    "now.okstate.edu",
    "ntp.colby.edu",
    "bonehed.lcs.mit.edu",
    "ntp-s1.cise.ufl.edu",

    // South Africa
    "ntp1.meraka.csir.co.za",
    "ntp.is.co.za",
    "ntp2.is.co.za",
    "igubu.saix.net",
    "ntp1.neology.co.za",
    "ntp2.neology.co.za",
    "tick.meraka.csir.co.za",
    "tock.meraka.csir.co.za",
    "ntp.time.org.za",
    "ntp1.meraka.csir.co.za",
    "ntp2.meraka.csir.co.za",
    "0.za.pool.ntp.org",
    "1.za.pool.ntp.org",
    "2.za.pool.ntp.org",
    "3.za.pool.ntp.org",

    // Italy
    "ntp0.ien.it",
    "ntp1.ien.it",
    "ntp2.ien.it",
    "ntp1.inrim.it",
    "ntp2.inrim.it",
    "0.it.pool.ntp.org",
    "1.it.pool.ntp.org",
    "2.it.pool.ntp.org",
    "3.it.pool.ntp.org",

    // Netherlands
    "ntp0.nl.net",
    "ntp1.nl.net",
    "ntp2.nl.net",
    "ntp.utwente.nl",
    "0.nl.pool.ntp.org",
    "1.nl.pool.ntp.org",
    "2.nl.pool.ntp.org",
    "3.nl.pool.ntp.org",

    // United Kingdom
    "ntp2d.mcc.ac.uk",
    "ntp2c.mcc.ac.uk",
    "ntp2b.mcc.ac.uk",
    "ntp.exnet.com",
    "ntp.cis.strath.ac.uk",
    "ntppub.le.ac.uk",
    "0.uk.pool.ntp.org",
    "1.uk.pool.ntp.org",
    "2.uk.pool.ntp.org",
    "3.uk.pool.ntp.org",

    // Canada
    "chime.utoronto.ca",
    "tick.utoronto.ca",
    "time.nrc.ca",
    "timelord.uregina.ca",
    "tock.utoronto.ca",
    "0.ca.pool.ntp.org",
    "1.ca.pool.ntp.org",
    "2.ca.pool.ntp.org",
    "3.ca.pool.ntp.org",

    // Japan
    "ntp.nict.jp",
    "0.jp.pool.ntp.org",
    "1.jp.pool.ntp.org",
    "2.jp.pool.ntp.org",
    "3.jp.pool.ntp.org",

    // Australia
    "ntp.cs.mu.oz.au",
    "augean.eleceng.adelaide.edu.au",
    "0.au.pool.ntp.org",
    "1.au.pool.ntp.org",
    "2.au.pool.ntp.org",
    "3.au.pool.ntp.org",

    // Slovenia
    "time.ijs.si",

    // Austria
    "0.at.pool.ntp.org",
    "1.at.pool.ntp.org",
    "2.at.pool.ntp.org",
    "3.at.pool.ntp.org",

    // ???
    "clepsydra.dec.com"
  };
  static size_t N_DEFAULT_SERVERS = 154;

  bool get_ntp_time(const std::string ntp_server, uint64_t ntp_port, time_t& ntp_time, time_t timeout_ms)
  {
    try {

      boost::asio::io_service io_service;
      udp::resolver resolver(io_service);
      udp::resolver::query query(udp::v4(), ntp_server, boost::lexical_cast<std::string>(ntp_port));
      udp::endpoint time_server_endpoint = *resolver.resolve(query);

      assert(sizeof(uint32_t) == 4);
      uint32_t buf[12] = {0};
      const uint32_t BUFSIZE = sizeof(buf);

      // open
      blocking_udp_client client;

      // request
      buf[0] = htonl((3 << 27) | (3 << 24));    // s. RFC 4330, http://www.apps.ietf.org/rfc/rfc4330.html
      if (!client.send_to(boost::asio::buffer(buf, BUFSIZE), time_server_endpoint))
      {
        LOG_ERROR("Error sending to " << ntp_server << ":" << ntp_port);
        return false;
      }

      // response
      boost::asio::ip::udp::endpoint here;
      if (!client.receive_from(boost::asio::buffer(buf, BUFSIZE), here, boost::posix_time::milliseconds(timeout_ms)))
      {
        LOG_ERROR("Error/timeout receiving from " << ntp_server << ":" << ntp_port);
        return false;
      }

      // process result
      time_t secs = ntohl(buf[8]) - 2208988800u;
      ntp_time = secs;
      return true;

    }
    catch (std::exception& e) {
      LOG_ERROR("std::exception in get_ntp_time: " << e.what());
      return false;
    }
    catch (...) {
      LOG_ERROR("Unknown exception in get_ntp_time");
      return false;
    }
  }

  class ntp_time::impl {
  public:
    impl(const std::vector<std::string>& ntp_servers, time_t refresh_time, time_t ntp_timeout_ms)
        : m_ntp_servers(ntp_servers), m_refresh_time(refresh_time), m_last_refresh(0)
        , m_ntp_minus_local(0), m_manual_delta(0), m_which_server(0)
        , m_ntp_timeout_ms(ntp_timeout_ms)
        , m_run(true)
        , m_check_update_thread(boost::bind(&impl::check_update_thread, this))
    {
      if (m_ntp_servers.empty())
      {
        throw std::runtime_error("Must provide at least one ntp server");
      }
      std::srand(time(NULL));
      std::random_shuffle(m_ntp_servers.begin(), m_ntp_servers.end());
    }

    ~impl()
    {
      m_run = false;
      m_check_update_thread.join();
    }

    bool should_update() const
    {
      CRITICAL_REGION_LOCAL(m_time_lock);
      return time(NULL) - m_last_refresh >= m_refresh_time;
    }

    void check_update_thread()
    {
      auto last = std::chrono::high_resolution_clock::now();
      while (m_run)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        auto now = std::chrono::high_resolution_clock::now();

        if (now < last || now - last > std::chrono::milliseconds(1500))
        {
          LOG_PRINT_YELLOW("System clock change detected, will update ntp time", LOG_LEVEL_0);
          CRITICAL_REGION_LOCAL(m_time_lock);
          m_last_refresh = 0;
        }
        last = now;
      }
      LOG_PRINT_L0("check_update_thread() finished");
    }

    std::vector<std::string> m_ntp_servers;
    time_t m_refresh_time;
    time_t m_last_refresh;

    time_t m_ntp_minus_local;
    time_t m_manual_delta;
    size_t m_which_server;

    time_t m_ntp_timeout_ms;

    mutable epee::critical_section m_time_lock;

    std::atomic<bool> m_run;
    std::thread m_check_update_thread;
  };

  ntp_time::ntp_time(const std::vector<std::string>& ntp_servers, time_t refresh_time, time_t ntp_timeout_ms)
      : m_pimpl(new impl(ntp_servers, refresh_time, ntp_timeout_ms))
  {
  }

  ntp_time::ntp_time(time_t refresh_time, time_t ntp_timeout_ms)
      : m_pimpl(new impl(std::vector<std::string>(DEFAULT_NTP_SERVERS, DEFAULT_NTP_SERVERS + N_DEFAULT_SERVERS),
                         refresh_time, ntp_timeout_ms))
  {
  }

  ntp_time::~ntp_time()
  {
  }

  bool ntp_time::update()
  {
    CRITICAL_REGION_LOCAL(m_pimpl->m_time_lock);
    LOG_PRINT_L2("Updating time from " << m_pimpl->m_ntp_servers[m_pimpl->m_which_server] << "...");

    time_t ntp_time;

    if (!get_ntp_time(m_pimpl->m_ntp_servers[m_pimpl->m_which_server], 123, ntp_time, m_pimpl->m_ntp_timeout_ms))
    {
      LOG_ERROR("Failed to get ntp time from " << m_pimpl->m_ntp_servers[m_pimpl->m_which_server] << ", rotating to next one");
      m_pimpl->m_which_server = (m_pimpl->m_which_server + 1) % m_pimpl->m_ntp_servers.size();
      return false;
    }

    time_t now = time(NULL);
    m_pimpl->m_ntp_minus_local = ntp_time - now;
    // don't affect manual delta
    m_pimpl->m_last_refresh = now;

    LOG_PRINT_L2("Local time is " << m_pimpl->m_ntp_minus_local << "s behind, plus manual delta of "
                 << m_pimpl->m_manual_delta << "s");
    return true;
  }

  void ntp_time::apply_manual_delta(time_t delta)
  {
    CRITICAL_REGION_LOCAL(m_pimpl->m_time_lock);
    m_pimpl->m_manual_delta += delta;
    LOG_PRINT_L2("Applying manual delta of " << delta << "s, total manual delta is now " << m_pimpl->m_manual_delta << "s");
  }

  void ntp_time::set_ntp_timeout_ms(time_t timeout_ms)
  {
    CRITICAL_REGION_LOCAL(m_pimpl->m_time_lock);
    m_pimpl->m_ntp_timeout_ms = timeout_ms;
    LOG_PRINT_L2("Timeout for getting NTP time is now " << timeout_ms << "ms");
  }

  time_t ntp_time::get_time()
  {
    CRITICAL_REGION_LOCAL(m_pimpl->m_time_lock);
    if (m_pimpl->should_update())
    {
      update();
    }

    return m_pimpl->m_ntp_minus_local + time(NULL) + m_pimpl->m_manual_delta;
  }
}
