// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#pragma once

namespace Hosting2
{
    class FirewallSecurityProvider : 
        public Common::RootedObject,
        public Common::FabricComponent,
        private Common::TextTraceComponent<Common::TraceTaskCodes::Hosting>
    {
        DENY_COPY(FirewallSecurityProvider)

    public:
        FirewallSecurityProvider(
            Common::ComponentRoot const & root);
        virtual ~FirewallSecurityProvider();

        Common::ErrorCode ConfigurePortFirewallPolicy(
            std::wstring const & policyName,
            std::vector<LONG> ports,
            uint64 nodeInstanceId);

        Common::ErrorCode RemoveFirewallRule(
            std::wstring const & policyName,
            std::vector<LONG> const& ports,
            uint64 nodeInstanceId);

    protected:
        virtual Common::ErrorCode OnOpen();
        virtual Common::ErrorCode OnClose();
        virtual void OnAbort();

    private:
#if defined(PLATFORM_UNIX)
        HRESULT AddRule(
            std::wstring const & ruleName,
            std::wstring const & ruleDescription,
            LONG currentProfileBitMask,
            LONG protocol,
            LONG port,
            bool outgoing);

        Common::ErrorCode CleanupRulesForAllProfiles(std::wstring const & policyName, vector<LONG> const& ports, uint64 nodeInstanceId);
#else
        HRESULT AddRule(
            Microsoft::WRL::ComPtr<INetFwRules> rules,
            std::wstring const & ruleName,
            std::wstring const & ruleDescription,
            LONG currentProfileBitMask,
            LONG protocol,
            LONG port,
            bool outgoing);

        Common::ErrorCode CleanupRulesForAllProfiles(std::wstring const & policyName, Microsoft::WRL::ComPtr<INetFwRules> rules, vector<LONG> const& ports, uint64 nodeInstanceId);
#endif


        static std::wstring GetFirewallRuleName(
            std::wstring const & policyName,
            bool outgoing,
            LONG profileType,
            LONG protocol,
            LONG port,
            uint64 nodeInstanceId);

    private:
        static LONG allProfiles_[3];
        std::vector<LONG> allprotocols_;
        std::vector<LONG> profilesEnabled_;

    };
}
