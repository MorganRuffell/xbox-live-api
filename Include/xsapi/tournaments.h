//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************
#pragma once
#include "xsapi/multiplayer.h"

namespace xbox {
    namespace services {
        class xbox_live_context_impl;

        namespace tournaments {

/// <summary>
/// Defines values used to indicate the state for a tournament.
/// </summary>
enum class tournament_state
{
    /// <summary>
    /// The Tournament state is unknown.
    /// </summary>
    unknown,

    /// <summary>
    /// Default. The Tournament is currently active.
    /// </summary>
    active,

    /// <summary>
    /// The Tournament was canceled.
    /// </summary>
    canceled,

    /// <summary>
    ///  The Tournament has been completed.
    /// </summary>
    completed
};

/// <summary>
/// The property used to order results by for retrieving a list of tournaments.
/// </summary>
enum class tournament_order_by
{
    /// <summary>
    /// No order provided.
    /// </summary>
    none,

    /// <summary>
    /// The only valid sort order for start_time is tournament_sort_order::ascending. 
    /// </summary>
    start_time,

    /// <summary>
    /// end_time is only set on completed tournaments so it is only valid to sort by end_time when 
    /// the state query parameter is set to tournament_state::completed.
    /// </summary>
    end_time
};

/// <summary>
/// The order in which to sort the results.
/// </summary>
enum class tournament_sort_order
{
    /// <summary>
    /// No sort order provided.
    /// </summary>
    none,

    /// <summary>
    /// Sorts smaller items first.
    /// </summary>
    ascending,

    /// <summary>
    /// Sorts larger items first.
    /// </summary>
    descending
};

/// <summary>
/// Defines values used to indicate the state of a team.
/// </summary>
enum class team_state
{
    /// <summary>
    /// The team state is unknown.
    /// </summary>
    unknown,

    /// <summary>
    /// The team is registered.
    /// </summary>
    registered,

    /// <summary>
    /// The team is currently on the wait list.
    /// </summary>
    waitlisted,

    /// <summary>
    /// The team is on stand by.
    /// </summary>
    stand_by,

    /// <summary>
    /// The team is checked in.
    /// </summary>
    checked_in,

    /// <summary>
    /// The team is playing.
    /// </summary>
    playing,

    /// <summary>
    /// The team is done with the tournament.
    /// Check the team_completed_reason state for more details.
    /// </summary>
    completed
};

/// <summary>
///
/// </summary>
enum class team_completed_reason
{
    /// <summary>
    /// The team completed reason is unknown.
    /// </summary>
    unknown,

    /// <summary>
    /// The team was rejected.
    /// </summary>
    rejected,

    /// <summary>
    /// The team was eliminated.
    /// </summary>
    eliminated,

    /// <summary>
    /// The team was evicted.
    /// </summary>
    evicted,

    /// <summary>
    /// The team has successfully completed the tournament.
    /// </summary>
    completed
};

/// <summary>
///
/// </summary>
enum class team_order_by
{
    /// <summary>
    /// No order provided.
    /// </summary>
    none,

    /// <summary>
    /// Order by name.
    /// </summary>
    name,

    /// <summary>
    /// Order by ranking.
    /// </summary>
    ranking,
};

class match_metadata
{
public:
    /// <summary>
    /// A formatted string that describes the team's standing in the tournament. For example '1W - 0L'.
    /// References a format string and provides values for the substitution tokens.
    /// </summary>
    _XSAPIIMP const string_t& label() const;

    /// <summary>
    /// The time this match is scheduled to start. 
    /// </summary>
    _XSAPIIMP const utility::datetime& start_time() const;

    /// <summary>
    /// True if the match is a bye which means that the opposing team does not play.
    /// </summary>
    _XSAPIIMP bool is_bye() const;

    /// <summary>
    /// The team IDs of all the opposing teams. This should be absent if bye is true.
    /// </summary>
    _XSAPIIMP const std::vector<string_t>& opposing_team_ids() const;

    /// <summary>
    /// Internal function
    /// </summary>
    match_metadata();

    /// <summary>
    /// Internal function
    /// </summary>
    static xbox::services::xbox_live_result<match_metadata> _Deserialize(_In_ const web::json::value& json);

private:
    string_t m_label;
    bool m_isBye;
    utility::datetime m_startTime;
    std::vector<string_t> m_opposingTeamIds;
};

/// <summary>
/// Metadata associated with the team's current or upcoming match. Absent if the team does not have an current or upcoming match.
/// </summary>
class current_match_metadata
{
public:

    /// <summary>
    /// The uniquely identifying information for the game session.
    /// </summary>
    _XSAPIIMP const xbox::services::multiplayer::multiplayer_session_reference& game_session_reference() const;

    /// <summary>
    /// 
    /// </summary>
    _XSAPIIMP const match_metadata& match_details() const;

    /// <summary>
    /// Internal function
    /// </summary>
    current_match_metadata();

    /// <summary>
    /// Internal function
    /// </summary>
    static xbox::services::xbox_live_result<current_match_metadata> _Deserialize(_In_ const web::json::value& json);

private:
    match_metadata m_matchDetails;
    xbox::services::multiplayer::multiplayer_session_reference m_gameSessionReference;
};

/// <summary>
/// Metadata associated with the team's previously played match. Absent if the player has not played any matches.
/// </summary>
class previous_match_metadata
{
public:

    /// <summary>
    /// The last game's result for the tournament.
    /// </summary>
    _XSAPIIMP xbox::services::tournaments::tournament_team_result result() const;

    /// <summary>
    /// The timestamp when the match ended.
    /// </summary>
    _XSAPIIMP const utility::datetime& match_end_time() const;

    /// <summary>
    /// 
    /// </summary>
    _XSAPIIMP const match_metadata& match_details() const;

    /// <summary>
    /// Internal function
    /// </summary>
    previous_match_metadata();

    /// <summary>
    /// Internal function
    /// </summary>
    static xbox::services::xbox_live_result<previous_match_metadata> _Deserialize(_In_ const web::json::value& json);

private:
    match_metadata m_matchDetails;
    utility::datetime m_endTime;
    xbox::services::tournaments::tournament_team_result m_result;
};

/// <summary>
/// Gets teams for a tournament based on the configuration of this request.
/// </summary>
class team_request
{
public:

    /// <summary>Creates a team_request object.</summary>
    /// <param name="organizerId">The ID of the tournament organizer.</param>
    /// <param name="tournamentId">The ID of the tournament.</param>
    _XSAPIIMP team_request(
        _In_ string_t organizerId,
        _In_ string_t tournamentId
        );

    /// <summary>
    /// The maximum number of items to return.
    /// </summary>
    /// <param name="maxItems">The maximum number of items to return.</param>
    _XSAPIIMP void set_max_items(_In_ uint32_t maxItems);

    /// <summary>
    /// Filter results based on the multiple states of the team.
    /// </summary>
    /// <param name="state">The team states to filter on.</param>
    _XSAPIIMP void set_state_filter(_In_ std::vector<team_state> states);

    /// <summary>
    /// Filter results based on the order specified.
    /// </summary>
    /// <param name="orderBy">The field used to order results.</param>
    _XSAPIIMP void set_order_by(_In_ team_order_by orderBy);

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP uint32_t _Max_items() const;

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP const std::vector<team_state>& _Team_states() const;

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP team_order_by _Order_by() const;

private:
    uint32_t m_maxItems;
    string_t m_organizerId;
    string_t m_tournamentId;
    team_order_by m_orderBy;
    std::vector<team_state> m_states;
};

/// <summary>
/// Represents 
/// </summary>
class team_info
{
public:
    /// <summary>
    /// The ID of the team. It is an opaque string specified by the tournament organizer.
    /// </summary>
    _XSAPIIMP const string_t& id() const;

    /// <summary>
    /// he display name of the team
    /// </summary>
    _XSAPIIMP const string_t& name() const;

    /// <summary>
    /// Member xbox_user_ids for this team.
    /// </summary>
    _XSAPIIMP const std::vector<string_t>& member_xbox_user_ids() const;

    /// <summary>
    /// The timestamp at which the team was registered. Used to determine ordering if team is waitlisted. 
    /// </summary>
    _XSAPIIMP const utility::datetime& registration_date() const;

    /// <summary>
    /// The state of the team.
    /// </summary>
    _XSAPIIMP team_state state() const;

    /// <summary>
    /// The reason the team is in the Completed state.
    /// </summary>
    _XSAPIIMP team_completed_reason completed_reason() const;

    /// <summary>
    /// The team's final rank within the tournament, if it is available. If missing, the rank is not available.
    /// </summary>
    _XSAPIIMP uint64_t ranking() const;

    /// <summary>
    /// Information about the team's current or upcoming match. Absent if the team does not have an current or upcoming match.
    /// </summary>
    _XSAPIIMP const xbox::services::tournaments::current_match_metadata& current_match_metadata() const;

    /// <summary>
    /// Information about the previously played match. Absent if the player has not played any matches.
    /// </summary>
    _XSAPIIMP const xbox::services::tournaments::previous_match_metadata& previous_match_metadata() const;

    /// <summary>
    /// Internal function
    /// </summary>
    static xbox::services::xbox_live_result<team_info> _Deserialize(_In_ const web::json::value& json);

    /// <summary>
    /// Internal function
    /// </summary>
    team_info();

private:
    static team_state _Convert_string_to_team_state(_In_ const string_t& value);
    static team_completed_reason _Convert_string_to_completed_reason(_In_ const string_t& value);

    string_t m_teamId;
    string_t m_teamName;
    std::vector<string_t> m_memberXuids;
    utility::datetime m_registrationDate;
    team_state m_state;
    team_completed_reason m_completedReason;
    uint64_t m_ranking;
    xbox::services::tournaments::current_match_metadata m_currentMatchMetadata;
    xbox::services::tournaments::previous_match_metadata m_previousMatchMetadata;
};

/// <summary>
/// Represents a collection of team_info class objects returned by a request.
/// </summary>
class team_request_result
{
public:

    /// <summary>
    /// A collection of team_info objects returned by a request.
    /// </summary>
    _XSAPIIMP const std::vector<team_info>& teams() const;

    /// <summary>
    /// Returns a boolean value that indicates if there are more pages of teams to retrieve.
    /// </summary>
    /// <returns>True if there are more pages, otherwise false.</returns>
    _XSAPIIMP bool has_next() const;

    /// <summary>
    /// Returns an team_request_result object that contains the next page of teams.
    /// </summary>
    /// <returns>An team_request_result object that contains a list of team_info objects.</returns>
    /// <remarks>
    /// Returns a concurrency::task<T> object that represents the state of the asynchronous operation.
    /// </remarks>
    _XSAPIIMP pplx::task<xbox::services::xbox_live_result<team_request_result>> get_next();

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP team_request_result();

    /// <summary>
    /// Internal function
    /// </summary>
    void _Init_next_page_info(
        _In_ std::shared_ptr<xbox::services::user_context> userContext,
        _In_ std::shared_ptr<xbox::services::xbox_live_context_settings> xboxLiveContextSettings,
        _In_ std::shared_ptr<xbox::services::xbox_live_app_config> appConfig
        );

    /// <summary>
    /// Internal function
    /// </summary>
    static xbox::services::xbox_live_result<team_request_result> _Deserialize(_In_ const web::json::value& json);

private:
    std::shared_ptr<xbox::services::user_context> m_userContext;
    std::shared_ptr<xbox::services::xbox_live_context_settings> m_xboxLiveContextSettings;
    std::shared_ptr<xbox::services::xbox_live_app_config> m_appConfig;

    std::vector<team_info> m_items;
    string_t m_nextLinkUrl;

};

/// <summary>
/// Gets tournaments based on the configuration of this request.
/// </summary>
class tournament_request
{
public:

    /// <summary>Creates a tournament_request object.</summary>
    /// <param name="filterResultsForUser">Filter results to only tournaments where this user is participating.</param>
    _XSAPIIMP tournament_request(
        _In_ bool filterResultsForUser = true
        );

    /// <summary>
    /// The maximum number of items to return.
    /// </summary>
    /// <param name="maxItems">The maximum number of items to return.</param>
    _XSAPIIMP void set_max_items(_In_ uint32_t maxItems);

    /// <summary>
    /// Filter results based on the multiple states of the tournament.
    /// </summary>
    /// <param name="state">The tournament states to filter on.</param>
    _XSAPIIMP void set_state_filter(_In_ std::vector<tournament_state> states);

    /// <summary>
    /// The property used to order results.
    /// </summary>
    /// <param name="orderBy">The tournament order to filter by.</param>
    _XSAPIIMP void set_order_by(_In_ tournament_order_by orderBy);

    /// <summary>
    ///The order in which to sort the results.
    /// </summary>
    /// <param name="sortOrder">The sort order to filter by.</param>
    _XSAPIIMP void set_sort_order(_In_ tournament_sort_order sortOrder);

    /// <summary>
    /// Filter results based on the organizer of the tournament.
    /// </summary>
    /// <param name="organizer">The organizer to filter by.</param>
    _XSAPIIMP void set_organizer_id(_In_ const string_t& organizer);

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP bool _Filter_results_for_user() const;

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP uint32_t _Max_items() const;

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP const std::vector<tournament_state>& _Tournament_states() const;

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP tournament_order_by _Order_by() const;

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP tournament_sort_order _Sort_order() const;

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP const string_t& _Organizer_id() const;

private:
    tournament_sort_order m_sortOrder;
    tournament_order_by m_orderBy;
    bool m_filterResultsForUser;
    uint32_t m_maxItems;
    string_t m_organizerId;
    std::vector<xbox::services::tournaments::tournament_state> m_states;
    
};

/// <summary>
/// Represents 
/// </summary>
class tournament
{
public:
    /// <summary>
    /// The ID of the tournament. It is an opaque string specified by the tournament organizer.
    /// </summary>
    _XSAPIIMP const string_t& id() const;

    /// <summary>
    /// The organizer of this tournament.
    /// </summary>
    _XSAPIIMP const string_t& organizer_id() const;

    /// <summary>
    /// A string representing the friendly name of the organizer
    /// </summary>
    _XSAPIIMP const string_t& organizer_name() const;

    /// <summary>
    /// A friendly name for the tournament, intended for display.
    /// </summary>
    _XSAPIIMP const string_t& name() const;

    /// <summary>
    /// An additional, longer description of the tournament, intended for display.
    /// </summary>
    _XSAPIIMP const string_t& description() const;

    /// <summary>
    /// A string representing the friendly name of the game mode
    /// </summary>
    _XSAPIIMP const string_t& game_mode() const;

    /// <summary>
    /// A string representing the style of the tournament, such as "Single Elimination" or "Round Robin"
    /// </summary>
    _XSAPIIMP const string_t& tournament_style() const;

    /// <summary>
    /// True when registration is open.
    /// </summary>
    _XSAPIIMP bool is_registration_open() const;

    /// <summary>
    /// True when check-in is open. 
    /// </summary>
    _XSAPIIMP bool is_checkin_open() const;

    /// <summary>
    /// True when play is open.
    /// </summary>
    _XSAPIIMP bool is_playing_open() const;

    /// <summary>
    /// True if the tournament has a prize. False otherwise.
    /// </summary>
    _XSAPIIMP bool has_prize() const;

    /// <summary>
    /// A flag indicating whether the tournament is currently paused.
    /// </summary>
    _XSAPIIMP bool is_paused() const;

    /// <summary>
    /// The min number of players required on a team.
    /// </summary>
    _XSAPIIMP uint32_t min_team_size() const;

    /// <summary>
    /// The max number of players that can play on a team.
    /// </summary>
    _XSAPIIMP uint32_t max_team_size() const;

    /// <summary>
    /// The number of teams registered for the tournament, not including waitlisted teams.
    /// </summary>
    _XSAPIIMP uint32_t teams_registered_count() const;

    /// <summary>
    /// This is the minimum number of registered teams. This does not include waitlisted teams.
    /// </summary>
    _XSAPIIMP uint32_t min_teams_registered() const;

    /// <summary>
    /// This is the maximum number of registered teams. This does not include waitlisted teams.
    /// </summary>
    _XSAPIIMP uint32_t max_teams_registered() const;

    /// <summary>
    /// The current state of the tournament. Tournaments are Active by default. 
    /// Organizers can set the tournament to Canceled or Completed by explicitly updating the tournament.
    /// </summary>
    _XSAPIIMP xbox::services::tournaments::tournament_state tournament_state() const;

    /// <summary>
    /// The start time of registration. Must be before checkinStart.
    /// </summary>
    _XSAPIIMP const utility::datetime& registration_start_time() const;

    /// <summary>
    /// The end time of registration. Must be before checkinStart.
    /// </summary>
    _XSAPIIMP const utility::datetime& registration_end_time() const;

    /// <summary>
    /// The start time of checkin. Must be before playingStart.
    /// </summary>
    _XSAPIIMP const utility::datetime& checkin_start_time() const;

    /// <summary>
    /// The end time of checkin. Must be before playingStart.
    /// </summary>
    _XSAPIIMP const utility::datetime& checkin_end_time() const;

    /// <summary>
    /// The time the tournament begins.
    /// </summary>
    _XSAPIIMP const utility::datetime& playing_start_time() const;

    /// <summary>
    /// The time the tournament ends.
    /// </summary>
    _XSAPIIMP const utility::datetime& playing_end_time() const;

    /// <summary>
    /// The datetime that this tournament has reached either the Canceled or Completed state. 
    /// This is set automatically when a tournament is updated to be Canceled or Complete. 
    /// </summary>
    _XSAPIIMP const utility::datetime& end_time() const;

    /// <summary>
    /// The ID of the team. It is an opaque string specified by the tournament organizer.
    /// </summary>
    _XSAPIIMP const string_t& team_id() const;

    /// <summary>
    /// The state of the team.
    /// </summary>
    _XSAPIIMP team_state team_state() const;

    /// <summary>
    /// The team's final rank within the tournament, if it is available. If missing, the rank is not available.
    /// </summary>
    _XSAPIIMP uint64_t team_ranking() const;

    /// <summary>
    /// Internal function
    /// </summary>
    tournament();

    /// <summary>
    /// Internal function
    /// </summary>
    static xbox::services::xbox_live_result<tournament> _Deserialize(
        _In_ const web::json::value& tournamentJson,
        _In_ const web::json::value& teamJson
        );

private:

    static xbox::services::tournaments::tournament_state _Convert_string_to_tournament_state(_In_ const string_t& value);

    bool m_isRegistrationOpen;
    bool m_isCheckinOpen;
    bool m_isPlayingOpen;
    bool m_hasPrize;
    bool m_isPaused;
    uint32_t m_minTeamSize;
    uint32_t m_maxTeamSize;
    uint32_t m_numTeamsRegistered;
    uint32_t m_minTeamsRegistered;
    uint32_t m_maxTeamsRegistered;
    string_t m_tournamentId;
    string_t m_organizerId;
    string_t m_organizerName;
    string_t m_name;
    string_t m_description;
    string_t m_gameMode;
    string_t m_tournamentStyle;
    utility::datetime m_registrationStartTime;
    utility::datetime m_registrationEndTime;
    utility::datetime m_checkinStartTime;
    utility::datetime m_checkinEndTime;
    utility::datetime m_playingStartTime;
    utility::datetime m_playingEndTime;
    utility::datetime m_tournamentEndTime;
    xbox::services::tournaments::tournament_state m_tournamentState;

    string_t m_teamId;
    uint64_t m_teamRanking;
    xbox::services::tournaments::team_state m_teamState;
};

/// <summary>
/// Represents a collection of tournament class objects returned by a request.
/// </summary>
class tournament_request_result
{
public:

    /// <summary>
    /// The collection of tournament objects returned by a request.
    /// </summary>
    _XSAPIIMP const std::vector<tournament>& tournaments() const;

    /// <summary>
    /// Returns a boolean value that indicates if there are more pages of Tournaments to retrieve.
    /// </summary>
    /// <returns>True if there are more pages, otherwise false.</returns>
    _XSAPIIMP bool has_next() const;

    /// <summary>
    /// Returns an tournament_request_result object that contains the next page of Tournament.
    /// </summary>
    /// <returns>An tournament_request_result object that contains a list of tournament objects.</returns>
    /// <remarks>
    /// Returns a concurrency::task<T> object that represents the state of the asynchronous operation.
    /// </remarks>
    _XSAPIIMP pplx::task<xbox::services::xbox_live_result<tournament_request_result>> get_next();

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP tournament_request_result();

    /// <summary>
    /// Internal function
    /// </summary>
    void _Init_next_page_info(
        _In_ std::shared_ptr<xbox::services::user_context> userContext,
        _In_ std::shared_ptr<xbox::services::xbox_live_context_settings> xboxLiveContextSettings,
        _In_ std::shared_ptr<xbox::services::xbox_live_app_config> appConfig
        );

    /// <summary>
    /// Internal function
    /// </summary>
    static xbox::services::xbox_live_result<tournament_request_result> _Deserialize(_In_ const web::json::value& json);

private:
    std::shared_ptr<xbox::services::user_context> m_userContext;
    std::shared_ptr<xbox::services::xbox_live_context_settings> m_xboxLiveContextSettings;
    std::shared_ptr<xbox::services::xbox_live_app_config> m_appConfig;

    std::vector<tournament> m_items;
    string_t m_nextLinkUrl;
};

/// <summary>
/// Represents an endpoint that you can use to access the Tournament service.
/// </summary>
class tournament_service
{
public:
    /// <summary>
    /// Returns a list of tournaments for the current title.
    /// </summary>
    /// <param name="request">A tournament request object that retrieves tournaments based on the configuration of the request.</param>
    /// <returns>A list of tournaments if it exist.</returns>
    /// <remarks>
    /// Returns a concurrency::task<T> object that represents the state of the asynchronous operation.
    /// This method calls GET /tournaments
    /// </remarks>
    _XSAPIIMP pplx::task<xbox::services::xbox_live_result<tournament_request_result>> get_tournaments(
        _In_ tournament_request request
        );

    /// <summary>
    /// Returns a specific tournament object.
    /// The tournament ID and the organizer ID together uniquely identify a tournament.
    /// </summary>
    /// <param name="organizerId">The ID of the tournament organizer.</param>
    /// <param name="tournamentId">The ID of the tournament.</param>
    /// <returns>The specific tournament if they exist.</returns>
    /// <remarks>
    /// Returns a concurrency::task<T> object that represents the state of the asynchronous operation.
    /// This method calls GET /tournaments/{organizer}/{id}.
    /// </remarks>
    _XSAPIIMP pplx::task<xbox::services::xbox_live_result<tournament>> get_tournament_details(
        _In_ const string_t& organizerId,
        _In_ const string_t& tournamentId
        );

    /// <summary>
    /// Returns a list of teams for a tournament.
    /// </summary>
    /// <param name="request">A team request object that retrieves team based on the configuration of the request.</param>
    /// <returns>A list of teams if they exist.</returns>
    /// <remarks>
    /// Returns a concurrency::task<T> object that represents the state of the asynchronous operation.
    /// This method calls GET /tournaments/{organizer}/{id}/teams
    /// </remarks>
    _XSAPIIMP pplx::task<xbox::services::xbox_live_result<team_request_result>> get_teams(
        _In_ team_request request
        );

    /// <summary>
    /// Returns a specific team object.
    /// The tournament ID and the organizer ID together uniquely identify a tournament.
    /// </summary>
    /// <param name="organizerId">The ID of the tournament organizer. This is case sensitive.</param>
    /// <param name="tournamentId">The ID of the tournament.</param>
    /// <param name="teamId">The ID of the team.</param>
    /// <returns>A specific tournament if they exist.</returns>
    /// <remarks>
    /// Returns a concurrency::task<T> object that represents the state of the asynchronous operation.
    /// This method calls GET /tournaments/{organizer}/{id}/teams/{teamId}
    /// </remarks>
    _XSAPIIMP pplx::task<xbox::services::xbox_live_result<team_info>> get_team_details(
        _In_ const string_t& organizerId,
        _In_ const string_t& tournamentId,
        _In_ const string_t& teamId
        );

    /// <summary>
    /// Internal function
    /// </summary>
    static xbox::services::tournaments::team_state _Convert_string_to_team_state(_In_ const string_t& value);

    /// <summary>
    /// Internal function
    /// </summary>
    pplx::task<xbox::services::xbox_live_result<tournament_request_result>> _Get_tournaments(
        _In_ const string_t& nextLinkUrl
        );

    /// <summary>
    /// Internal function
    /// </summary>
    _XSAPIIMP pplx::task<xbox::services::xbox_live_result<team_request_result>> _Get_teams(
        _In_ const string_t& nextLinkUrl
        );

    /// <summary>
    /// Internal function
    /// </summary>
    tournament_service();

    /// <summary>
    /// Internal function
    /// </summary>
    tournament_service(
        _In_ std::shared_ptr<xbox::services::user_context> userContext,
        _In_ std::shared_ptr<xbox::services::xbox_live_context_settings> xboxLiveContextSettings,
        _In_ std::shared_ptr<xbox::services::xbox_live_app_config> appConfig
        );

private:

    pplx::task<xbox::services::xbox_live_result<tournament_request_result>> get_tournaments_internal(_In_ const string_t& subPath);

    string_t tournament_sub_path_url(_In_ tournament_request getTournamentsRequest);

    string_t convert_state_to_string(_In_ tournament_state state);
    string_t convert_sort_order_to_string(_In_ tournament_sort_order order);
    string_t convert_order_by_to_string(_In_ tournament_order_by order);

    std::shared_ptr<xbox::services::user_context> m_userContext;
    std::shared_ptr<xbox::services::xbox_live_context_settings> m_xboxLiveContextSettings;
    std::shared_ptr<xbox::services::xbox_live_app_config> m_appConfig;
};

}}}