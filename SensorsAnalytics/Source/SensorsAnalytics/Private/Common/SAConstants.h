// Copyright 2022 SensorsData. All Rights Reserved.
#pragma once

class FSAConstants
{
public:

    constexpr static char const* const SA_PC_SDK_VERSION = "0.0.3";
    constexpr static char const* const SA_PC_SDK_NAME = "UnrealEngine";

    constexpr static char const* const SA_PLUGIN_VERSION = "unreal_engine:0.0.3";
    constexpr static char const* const SA_PLUGIN_VERSION_KEY = "$lib_plugin_version";

	constexpr static char const* const SA_EVENT_TYPE = "type";
	constexpr static char const* const SA_EVENT_NAME = "event";
    constexpr static char const* const SA_EVENT_TIME = "time";
    constexpr static char const* const SA_EVENT_ANONYMOUS_ID = "anonymous_id";
    constexpr static char const* const SA_EVENT_DISTINCT_ID = "distinct_id";
    constexpr static char const* const SA_EVENT_IDENTITIES = "identities";
    constexpr static char const* const SA_EVENT_LOGIN_ID = "login_id";
    constexpr static char const* const SA_EVENT_ORIGINAL_ID = "original_id";

    constexpr static char const* const SA_EVENT_FLUSH_TIME = "_flush_time";
    constexpr static char const* const SA_EVENT_TRACK_ID = "_track_id";
    constexpr static char const* const SA_EVENT_PROPERTIES = "properties";
    constexpr static char const* const SA_EVENT_LIB = "lib";

    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_NETWORK_TYPE = "$network_type";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_OS = "$os";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_OS_VERSION = "$os_version";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_SCREEN_WIDTH = "$screen_width";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_SCREEN_HEIGHT = "$screen_height";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_APP_ID = "$app_id";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_APP_VERSION = "$app_version";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_IS_FIRST_DAY = "$is_first_day";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_FIRST_VISIT_TIME = "$first_visit_time";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_LIB = "$lib";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_LIB_VERSION = "$lib_version";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_LIB_METHOD = "$lib_method";

    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_TIMEZONE_OFFSET = "$timezone_offset";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_DEVICE_ID = "$device_id";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_INSTALL_DISABLE_CALLBACK = "$ios_install_disable_callback";

    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_DATA_INGESTION_SOURCE = "$data_ingestion_source";

    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_IDENTITY_LOGIN_ID = "$identity_login_id";

    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_IDENTITY_MAC_SERIAL_ID = "$identity_mac_serial_id";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_IDENTITY_MAC_UUID = "$identity_mac_uuid";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_IDENTITY_WINDOWS_SERIAL_ID = "$identity_windows_serial_id";
    constexpr static char const* const SA_EVENT_PRESET_PROPERTY_IDENTITY_WINDOWS_UUID = "$identity_windows_uuid";

    constexpr static char const* const SA_EVENT_NAME_SIGN_UP = "$SignUp";
    constexpr static char const* const SA_EVENT_NAME_APP_INSTALL = "$AppInstall";

  
	const static uint32 SA_SLOT_USER_INDEX_CONFIG = 86;
	//constexpr static char const* const SA_SLOT_SAVE_CONFIG_SUFFIX = ".SAConfig";
	const static uint32 SA_SLOT_USER_INDEX_EVENT = 75;
	constexpr static char const* const SA_SLOT_SAVE_EVENT_SUFFIX = ".SAEvent";

	//EVENT TYPE
   	constexpr static char const* const SA_EVENT_TYPE_TRACK = "track";
    constexpr static char const* const SA_EVENT_TYPE_TRACK_SIGNUP = "track_signup";

   	constexpr static char const* const SA_EVENT_TYPE_PROFILE_SET = "profile_set";
   	constexpr static char const* const SA_EVENT_TYPE_PROFILE_SET_ONCE = "profile_set_once";
    constexpr static char const* const SA_EVENT_TYPE_PROFILE_UNSET = "profile_unset";
    constexpr static char const* const SA_EVENT_TYPE_PROFILE_DELETE = "profile_delete";
    constexpr static char const* const SA_EVENT_TYPE_PROFILE_APPEND = "profile_append";
    constexpr static char const* const SA_EVENT_TYPE_PROFILE_INCREMENT = "profile_increment";
    
    // EventName & PropertyKey max length
    const static uint32 SA_EVENT_NAME_MAX_LENGTH = 100;
    const static uint32 SA_PROPERTY_VALUE_MAX_LENGTH = 1024;
};
