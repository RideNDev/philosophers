#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <fcntl.h>
# include <semaphore.h>
# include <stdint.h>

# define MSG "msg_w"
# define FORK "fork_w"
# define END "end_w"

typedef struct		s_philo
{
	sem_t			*eat;
	int				name;
	int				last_eat;
	int				nb_of_eat;
}					t_philo;

typedef struct		s_data
{
	t_philo			*philo;
	sem_t			*fork;
	sem_t			*msg;
	sem_t			*end;
	int				last_msg;
	int				nb;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_time_each_philosophers_must_eat;
	int				philo_ok;
	int				time_start;
	int				philo_dead;
}					t_data;

extern t_data		*g_data;

int					init_game(int ac, char **av);
int					init_game2();
void				init_philosophers(void);
void				clean();
int					start_game(void);
int					end_game();
void				*ft_philo(void *tmp_philo);
void				philo_eat(t_philo *philo);
void				*check_life(void *tmp_philo);
void				message(t_philo *philo, int msg);
int					get_time(void);
sem_t				*ft_sem(const char *str, int nb);
int					ft_atoi(const char *str);
void				ft_putnbr_fd(int nb, int fd);
char				*ft_itoa(int n);

#endif
