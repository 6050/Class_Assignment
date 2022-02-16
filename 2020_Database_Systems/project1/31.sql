SELECT type AS '진화 가능한 포켓몬의 수가 3개 이상인 타입'
FROM Pokemon AS P, Evolution AS E
WHERE id = E.before_id
GROUP BY type
HAVING COUNT(*) >=3
ORDER BY type DESC;