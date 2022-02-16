SELECT type AS '포켓몬 타입', COUNT(*) AS '포켓몬 수'
FROM Pokemon
GROUP BY type
ORDER BY COUNT(*), type;