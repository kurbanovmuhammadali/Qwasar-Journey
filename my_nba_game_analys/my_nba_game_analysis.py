import re
from collections import defaultdict

# --- helpers -------------------------------------------------------

def _blank_stats():
    """Return a new empty stat line for a player."""
    return {
        "FG": 0, "FGA": 0, "FG%": 0.0,
        "3P": 0, "3PA": 0, "3P%": 0.0,
        "FT": 0, "FTA": 0, "FT%": 0.0,
        "ORB": 0, "DRB": 0, "TRB": 0,
        "AST": 0, "STL": 0, "BLK": 0,
        "TOV": 0, "PF": 0, "PTS": 0
    }


def _compute_percent(made, attempts):
    return round(made / attempts, 3) if attempts else 0.0


# --- main analysis function ----------------------------------------

def analyse_nba_game(play_by_play_moves):
    """
    Parse the play-by-play list and return:
    {
      'home_team': {'name': ..., 'players_data': [...]},
      'away_team': {'name': ..., 'players_data': [...]}
    }
    """
    home_team_name = away_team_name = None
    home_stats = defaultdict(_blank_stats)
    away_stats = defaultdict(_blank_stats)

    for row in play_by_play_moves:
        # Split the row
        parts = row.strip().split('|')
        if len(parts) < 8:
            continue

        period, remaining, relevant_team, away_team, home_team, away_score, home_score, desc = parts
        if home_team_name is None:
            home_team_name = home_team
            away_team_name = away_team

        desc = desc.strip()

        # Determine which team and which player
        team_stats = home_stats if relevant_team == home_team_name else away_stats

        # -----------------------
        # Recognize events
        # -----------------------
        # Field goal made
        m = re.match(r'(.+) makes (3|2)-pt', desc)
        if m:
            player = m.group(1).strip()
            pts = int(m.group(2))
            team_stats[player]["FG"] += 1
            team_stats[player]["FGA"] += 1
            if pts == 3:
                team_stats[player]["3P"] += 1
                team_stats[player]["3PA"] += 1
            team_stats[player]["PTS"] += pts

            # Assist credit
            assist_m = re.search(r'assist by ([^)]+)', desc)
            if assist_m:
                assister = assist_m.group(1).strip()
                team_stats[assister]["AST"] += 1
            continue

        # Field goal missed
        m = re.match(r'(.+) misses (3|2)-pt', desc)
        if m:
            player = m.group(1).strip()
            pts = int(m.group(2))
            team_stats[player]["FGA"] += 1
            if pts == 3:
                team_stats[player]["3PA"] += 1
            continue

        # Free throws
        m = re.match(r'(.+) makes free throw', desc)
        if m:
            player = m.group(1).strip()
            team_stats[player]["FT"] += 1
            team_stats[player]["FTA"] += 1
            team_stats[player]["PTS"] += 1
            continue

        m = re.match(r'(.+) misses free throw', desc)
        if m:
            player = m.group(1).strip()
            team_stats[player]["FTA"] += 1
            continue

        # Rebounds
        m = re.match(r'Offensive rebound by (.+)', desc)
        if m:
            player = m.group(1).strip()
            team_stats[player]["ORB"] += 1
            team_stats[player]["TRB"] += 1
            continue

        m = re.match(r'Defensive rebound by (.+)', desc)
        if m:
            player = m.group(1).strip()
            team_stats[player]["DRB"] += 1
            team_stats[player]["TRB"] += 1
            continue

        # Turnover
        m = re.match(r'Turnover by (.+)', desc)
        if m:
            player = m.group(1).strip()
            team_stats[player]["TOV"] += 1
            continue

        # Steal
        m = re.search(r'steal by (.+)', desc)
        if m:
            player = m.group(1).strip()
            team_stats[player]["STL"] += 1
            continue

        # Block
        m = re.search(r'block by (.+)', desc)
        if m:
            player = m.group(1).strip()
            team_stats[player]["BLK"] += 1
            continue

        # Foul
        m = re.match(r'Shooting foul by (.+)', desc)
        if m:
            player = m.group(1).strip()
            team_stats[player]["PF"] += 1
            continue

    # Compute percentages
    def finalize(stats_dict):
        players_data = []
        for player, st in stats_dict.items():
            st["FG%"] = _compute_percent(st["FG"], st["FGA"])
            st["3P%"] = _compute_percent(st["3P"], st["3PA"])
            st["FT%"] = _compute_percent(st["FT"], st["FTA"])
            players_data.append({"player_name": player, **st})
        return players_data

    return {
        "home_team": {"name": home_team_name, "players_data": finalize(home_stats)},
        "away_team": {"name": away_team_name, "players_data": finalize(away_stats)},
    }


# --- printing function ---------------------------------------------

def print_nba_game_stats(team_dict):
    """
    Print one team’s players_data in a nice table format.
    """
    header = [
        "Players","FG","FGA","FG%","3P","3PA","3P%","FT","FTA","FT%","ORB","DRB","TRB",
        "AST","STL","BLK","TOV","PF","PTS"
    ]
    print("\t".join(header))

    totals = _blank_stats()

    for player in team_dict["players_data"]:
        row = [player["player_name"]]
        for stat in header[1:]:
            val = player[stat] if stat in player else ""
            row.append(str(val))
            if stat in totals and stat not in ("FG%","3P%","FT%"):
                totals[stat] += player[stat]
        print("\t".join(row))

    # Totals line with percents recalculated
    totals["FG%"] = _compute_percent(totals["FG"], totals["FGA"])
    totals["3P%"] = _compute_percent(totals["3P"], totals["3PA"])
    totals["FT%"] = _compute_percent(totals["FT"], totals["FTA"])

    row = ["Team Totals"] + [str(totals.get(stat,"")) for stat in header[1:]]
    print("\t".join(row))


# --- Example usage -------------------------------------------------
if __name__ == "__main__":
    # paste your play_by_play_moves list here to test
    play_by_play_moves = [
        # ... (rows from your example)
    ]
    summary = analyse_nba_game(play_by_play_moves)
    print("\nHOME TEAM STATS:")
    print_nba_game_stats(summary["home_team"])
    print("\nAWAY TEAM STATS:")
    print_nba_game_stats(summary["away_team"])
